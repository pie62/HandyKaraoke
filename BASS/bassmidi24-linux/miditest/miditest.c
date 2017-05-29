/*
	BASSMIDI test player
	Copyright (c) 2006-2017 Un4seen Developments Ltd.
*/

#include <gtk/gtk.h>
#include <glade/glade.h>
#include <glib/gthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <regex.h>
#include <glob.h>
#include "bass.h"
#include "bassmidi.h"

// path to glade file
#ifndef GLADE_PATH
#define GLADE_PATH ""
#endif

GladeXML *glade;
GtkWidget *win=0;
GtkWidget *filesel,*fontfilesel;

HSTREAM chan;		// channel handle
HSOUNDFONT font;	// soundfont

float speed=1;	// tempo adjustment

char lyrics[1000]; // lyrics buffer

// display error messages
void Error(const char *es)
{
	GtkWidget *dialog=gtk_message_dialog_new(GTK_WINDOW(win),GTK_DIALOG_DESTROY_WITH_PARENT,
		GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,"%s\n(error code: %d)",es,BASS_ErrorGetCode());
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

#define GetWidget(id) glade_xml_get_widget(glade,id)

void WindowDestroy(GtkObject *obj, gpointer data)
{
	gtk_main_quit();
}

void CALLBACK LyricSync(HSYNC handle, DWORD channel, DWORD data, void *user)
{
	BASS_MIDI_MARK mark;
	const char *text;
	char *p;
	int lines;
	BASS_MIDI_StreamGetMark(channel,(intptr_t)user,data,&mark); // get the lyric/text
	text=mark.text;
	if (text[0]=='@') return; // skip info
	if (text[0]=='\\') { // clear display
		p=lyrics;
		text++;
	} else {
		p=lyrics+strlen(lyrics);
		if (text[0]=='/') { // new line
			*p++='\n';
			text++;
		}
	}
	sprintf(p,"%.*s",lyrics+sizeof(lyrics)-p-1,text); // add the text to the lyrics buffer
	for (lines=1,p=lyrics;p=strchr(p,'\n');lines++,p++) ; // count lines
	if (lines>3) { // remove old lines so that new lines fit in display...
		int a;
		for (a=0,p=lyrics;a<lines-3;a++) p=strchr(p,'\n')+1;
		strcpy(lyrics,p);
	}
	// update the lyrics display
	gdk_threads_enter();
	gtk_label_set_text(GTK_LABEL(GetWidget("lyrics")),lyrics);
	gdk_threads_leave();
}

void CALLBACK EndSync(HSYNC handle, DWORD channel, DWORD data, void *user)
{
	lyrics[0]=0; // clear lyrics
	gdk_threads_enter();
	gtk_label_set_text(GTK_LABEL(GetWidget("lyrics")),lyrics);
	gdk_threads_leave();
}

// look for a marker (eg. loop points)
BOOL FindMarker(HSTREAM handle, const char *text, BASS_MIDI_MARK *mark)
{
	int a;
	for (a=0;BASS_MIDI_StreamGetMark(handle,BASS_MIDI_MARK_MARKER,a,mark);a++) {
		if (!strcasecmp(mark->text,text)) return TRUE; // found it
	}
	return FALSE;
}

void CALLBACK LoopSync(HSYNC handle, DWORD channel, DWORD data, void *user)
{
	BASS_MIDI_MARK mark;
	if (FindMarker(channel,"loopstart",&mark)) // found a loop start point
		BASS_ChannelSetPosition(channel,mark.pos,BASS_POS_BYTE|BASS_MIDI_DECAYSEEK); // rewind to it (and let old notes decay)
	else
		BASS_ChannelSetPosition(channel,0,BASS_POS_BYTE|BASS_MIDI_DECAYSEEK); // else rewind to the beginning instead
}

gboolean FileExtensionFilter(const GtkFileFilterInfo *info, gpointer data)
{
	return !regexec((regex_t*)data,info->filename,0,NULL,0);
}

void OpenClicked(GtkButton *obj, gpointer data)
{
	int resp=gtk_dialog_run(GTK_DIALOG(filesel));
	gtk_widget_hide(filesel);
	if (resp==GTK_RESPONSE_ACCEPT) {
		char *file=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(filesel));
		BASS_StreamFree(chan); // free old stream before opening new
		gtk_label_set_text(GTK_LABEL(GetWidget("lyrics")),""); // clear lyrics display
		if (!(chan=BASS_MIDI_StreamCreateFile(FALSE,file,0,0,
				BASS_SAMPLE_FLOAT|BASS_SAMPLE_LOOP|BASS_MIDI_DECAYSEEK|(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(GetWidget("fx")))?0:BASS_MIDI_NOFX),1))) {
			// it ain't a MIDI
			gtk_button_set_label(obj,"click here to open a file...");
			gtk_label_set_text(GTK_LABEL(GetWidget("info")),"");
			gtk_label_set_text(GTK_LABEL(GetWidget("tick")),"-");
			Error("Can't play the file");
		} else {
			gtk_button_set_label(obj,file);
			{ // set the title (track name of first track)
				BASS_MIDI_MARK mark;
				if (BASS_MIDI_StreamGetMark(chan,BASS_MIDI_MARK_TRACK,0,&mark) && !mark.track)
					gtk_label_set_text(GTK_LABEL(GetWidget("info")),mark.text);
				else
					gtk_label_set_text(GTK_LABEL(GetWidget("info")),"");
			}
			// update pos scroller range (using tick length)
			gtk_range_set_range(GTK_RANGE(GetWidget("position")),0,(BASS_ChannelGetLength(chan,BASS_POS_MIDI_TICK)-1)/120);
			{ // set looping syncs
				BASS_MIDI_MARK mark;
				if (FindMarker(chan,"loopend",&mark)) // found a loop end point
					BASS_ChannelSetSync(chan,BASS_SYNC_POS|BASS_SYNC_MIXTIME,mark.pos,LoopSync,0); // set a sync there
				BASS_ChannelSetSync(chan,BASS_SYNC_END|BASS_SYNC_MIXTIME,0,LoopSync,0); // set one at the end too (eg. in case of seeking past the loop point)
			}
			{ // clear lyrics buffer and set lyrics syncs
				BASS_MIDI_MARK mark;
				lyrics[0]=0;
				if (BASS_MIDI_StreamGetMark(chan,BASS_MIDI_MARK_LYRIC,0,&mark)) // got lyrics
					BASS_ChannelSetSync(chan,BASS_SYNC_MIDI_MARK,BASS_MIDI_MARK_LYRIC,LyricSync,(void*)BASS_MIDI_MARK_LYRIC);
				else if (BASS_MIDI_StreamGetMark(chan,BASS_MIDI_MARK_TEXT,20,&mark)) // got text instead (over 20 of them)
					BASS_ChannelSetSync(chan,BASS_SYNC_MIDI_MARK,BASS_MIDI_MARK_TEXT,LyricSync,(void*)BASS_MIDI_MARK_TEXT);
				BASS_ChannelSetSync(chan,BASS_SYNC_END,0,EndSync,0);
			}
			BASS_MIDI_StreamEvent(chan,0,MIDI_EVENT_SPEED,speed*10000); // apply tempo adjustment
			{ // get default soundfont in case of matching soundfont being used
				BASS_MIDI_FONT sf;
				BASS_MIDI_StreamGetFonts(chan,&sf,1);
				font=sf.font;
			}
			// limit CPU usage to 70% (also enables async sample loading) and start playing
			BASS_ChannelSetAttribute(chan,BASS_ATTRIB_MIDI_CPU,70);
			BASS_ChannelPlay(chan,FALSE);
		}
		g_free(file);
	}
}

void OpenFontClicked(GtkButton *obj, gpointer data)
{
	int resp=gtk_dialog_run(GTK_DIALOG(fontfilesel));
	gtk_widget_hide(fontfilesel);
	if (resp==GTK_RESPONSE_ACCEPT) {
		char *file=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fontfilesel));
		HSOUNDFONT newfont=BASS_MIDI_FontInit(file,0);
		if (newfont) {
			BASS_MIDI_FONT sf;
			sf.font=newfont;
			sf.preset=-1; // use all presets
			sf.bank=0; // use default bank(s)
			BASS_MIDI_StreamSetFonts(0,&sf,1); // set default soundfont
			BASS_MIDI_StreamSetFonts(chan,&sf,1); // set for current stream too
			BASS_MIDI_FontFree(font); // free old soundfont
			font=newfont;
		}
		g_free(file);
	}
}

void FXToggled(GtkToggleButton *obj, gpointer data)
{ // toggle FX processing
	if (obj->active)
		BASS_ChannelFlags(chan,0,BASS_MIDI_NOFX); // enable FX
	else
		BASS_ChannelFlags(chan,BASS_MIDI_NOFX,BASS_MIDI_NOFX); // disable FX
}

gboolean PositionChange(GtkRange *range, GtkScrollType scroll, gdouble value, gpointer data)
{
	BASS_ChannelSetPosition(chan,value*120,BASS_POS_MIDI_TICK);
	lyrics[0]=0; // clear lyrics
	gtk_label_set_text(GTK_LABEL(GetWidget("lyrics")),lyrics);
	return FALSE;
}

void TempoChanged(GtkRange *range, gpointer data)
{
	int value=gtk_range_get_value(range);
	speed=(20+value)/20.f; // up to +/- 50% bpm
	BASS_MIDI_StreamEvent(chan,0,MIDI_EVENT_SPEED,speed*10000); // apply tempo adjustment
}

void VoicesChanged(GtkRange *range, gpointer data)
{
	int value=gtk_range_get_value(range);
	BASS_SetConfig(BASS_CONFIG_MIDI_VOICES,value); // set default voice limit
	if (chan) BASS_ChannelSetAttribute(chan,BASS_ATTRIB_MIDI_VOICES,value); // apply to current MIDI file too
}

gboolean TimerProc(gpointer data)
{
	char text[16];
	float active=0;
	if (chan) {
		DWORD tick=BASS_ChannelGetPosition(chan,BASS_POS_MIDI_TICK); // get position in ticks
		int tempo=BASS_MIDI_StreamGetEvent(chan,0,MIDI_EVENT_TEMPO); // get the file's tempo
		sprintf(text,"%u",tick);
		gtk_label_set_text(GTK_LABEL(GetWidget("tick")),text); // display position
		gtk_range_set_value(GTK_RANGE(GetWidget("position")),tick/120); // update position bar
		sprintf(text,"%.1f",speed*60000000.0/tempo); // calculate bpm
		gtk_label_set_text(GTK_LABEL(GetWidget("tempovalue")),text); // display it
		BASS_ChannelGetAttribute(chan,BASS_ATTRIB_MIDI_VOICES_ACTIVE,&active); // get active voices
	}
	sprintf(text,"%u / %u",(int)active,BASS_GetConfig(BASS_CONFIG_MIDI_VOICES));
	gtk_label_set_text(GTK_LABEL(GetWidget("voicetext")),text); // display voices
	sprintf(text,"%d%%",(int)BASS_GetCPU());
	gtk_label_set_text(GTK_LABEL(GetWidget("cpu")),text); // display CPU usage
	{
		static int updatefont=0;
		if (++updatefont&1) { // only updating font info once a second
			char text[80]="no soundfont";
			BASS_MIDI_FONTINFO i;
			if (BASS_MIDI_FontGetInfo(font,&i))
				snprintf(text,sizeof(text),"name: %s\nloaded: %d / %d",i.name,i.samload,i.samsize);
			gtk_label_set_text(GTK_LABEL(GetWidget("fontinfo")),text);
		}
	}
	return TRUE;
}

int main(int argc, char* argv[])
{
#if !GLIB_CHECK_VERSION(2,32,0)
	g_thread_init(NULL);
#endif
	gdk_threads_init();
	gtk_init(&argc,&argv);

	// check the correct BASS was loaded
	if (HIWORD(BASS_GetVersion())!=BASSVERSION) {
		Error("An incorrect version of BASS was loaded");
		return 0;
	}

	// initialize default output device
	if (!BASS_Init(-1,44100,0,NULL,NULL)) {
		Error("Can't initialize device");
		return 0;
	}

	// initialize GUI
	glade=glade_xml_new(GLADE_PATH"miditest.glade",NULL,NULL);
	if (!glade) return 0;
	win=GetWidget("window1");
	if (!win) return 0;
	glade_xml_signal_autoconnect(glade);

	{ // setup file selectors
		GtkFileFilter *filter;
		regex_t *fregex;
		filesel=gtk_file_chooser_dialog_new("Open File",GTK_WINDOW(win),GTK_FILE_CHOOSER_ACTION_OPEN,
			GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,GTK_STOCK_OPEN,GTK_RESPONSE_ACCEPT,NULL);
		filter=gtk_file_filter_new();
		gtk_file_filter_set_name(filter,"MIDI files (mid/midi/rmi/kar)");
		fregex=malloc(sizeof(*fregex));
		regcomp(fregex,"\\.(mid|midi|rmi|kar)$",REG_ICASE|REG_NOSUB|REG_EXTENDED);
		gtk_file_filter_add_custom(filter,GTK_FILE_FILTER_FILENAME,FileExtensionFilter,fregex,NULL);
		gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(filesel),filter);
		filter=gtk_file_filter_new();
		gtk_file_filter_set_name(filter,"All files");
		gtk_file_filter_add_pattern(filter,"*");
		gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(filesel),filter);

		fontfilesel=gtk_file_chooser_dialog_new("Open Soundfont",GTK_WINDOW(win),GTK_FILE_CHOOSER_ACTION_OPEN,
			GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,GTK_STOCK_OPEN,GTK_RESPONSE_ACCEPT,NULL);
		filter=gtk_file_filter_new();
		gtk_file_filter_set_name(filter,"Soundfonts (sf2/sf2pack)");
		fregex=malloc(sizeof(*fregex));
		regcomp(fregex,"\\.(sf2|sf2pack)$",REG_ICASE|REG_NOSUB|REG_EXTENDED);
		gtk_file_filter_add_custom(filter,GTK_FILE_FILTER_FILENAME,FileExtensionFilter,fregex,NULL);
		gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(fontfilesel),filter);
		filter=gtk_file_filter_new();
		gtk_file_filter_set_name(filter,"All files");
		gtk_file_filter_add_pattern(filter,"*");
		gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(fontfilesel),filter);
	}

	gtk_range_set_value(GTK_RANGE(GetWidget("voices")),BASS_GetConfig(BASS_CONFIG_MIDI_VOICES)); // get default voice limit

	// load optional plugins for packed soundfonts (others may be used too)
	BASS_PluginLoad("libbassflac.so",0);
	BASS_PluginLoad("libbasswv.so",0);

	g_timeout_add(500,TimerProc,NULL);

	gdk_threads_enter();
	gtk_main();
	gdk_threads_leave();

	gtk_widget_destroy(filesel);
	gtk_widget_destroy(fontfilesel);

	// free the output device and all plugins
	BASS_Free();
	BASS_PluginFree(0);

    return 0;
}
