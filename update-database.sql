PRAGMA foreign_keys = 0;

CREATE TABLE sqlitestudio_temp_table AS SELECT *
                                          FROM songs;

DROP TABLE songs;

DROP INDEX id_idx;
DROP INDEX name_idx;
DROP INDEX artist_idx;
DROP INDEX compound_idx;

CREATE TABLE songs (
    id       TEXT    COLLATE NOCASE,
    name     TEXT    COLLATE NOCASE,
    artist   TEXT    COLLATE NOCASE,
    keyname  TEXT,
    tempo    INTEGER,
    songtype TEXT,
    lyrics   TEXT,
    path     TEXT
);

CREATE TABLE IF NOT EXISTS miscellaneous (
    name TEXT,
    value_str TEXT,
    value_num INTEGER
);

INSERT INTO songs (
                      id,
                      name,
                      artist,
                      keyname,
                      tempo,
                      songtype,
                      lyrics,
                      path
                  )
                  SELECT id,
                         name,
                         artist,
                         keyname,
                         tempo,
                         songtype,
                         lyrics,
                         path
                    FROM sqlitestudio_temp_table;

DROP TABLE sqlitestudio_temp_table;

CREATE INDEX id_idx ON songs (
    id
);

CREATE INDEX name_idx ON songs (
    name
);

CREATE INDEX artist_idx ON songs (
    artist
);

CREATE INDEX compound_idx ON songs (
    id,
    name,
    artist
);

PRAGMA foreign_keys = 1;