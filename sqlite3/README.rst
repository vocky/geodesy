QuickStart
==========
some test about sqlite3 fts5.

FTS5 INSTALL
------------
+ readline install.
+ icu install.
+ sqlite3 src download.

export CFLAGS:

    $ export CFLAGS="-I/usr/local/include \
    -DSQLITE_ENABLE_COLUMN_METADATA=1 \
    -DSQLITE_ENABLE_DBSTAT_VTAB=1 \
    -DSQLITE_ENABLE_FTS3=1 \
    -DSQLITE_ENABLE_FTS3_PARENTHESIS=1 \
    -DSQLITE_ENABLE_FTS5=1 \
    -DSQLITE_ENABLE_JSON1=1 \
    -DSQLITE_ENABLE_RTREE=1 \
    -DSQLITE_ENABLE_UNLOCK_NOTIFY \
    -DSQLITE_ENABLE_UPDATE_DELETE_LIMIT \
    -DSQLITE_SECURE_DELETE \
    -DSQLITE_SOUNDEX \
    -DSQLITE_TEMP_STORE=3 \
    -DSQLITE_ENABLE_ICU \
    -fPIC"

set LIBS:
    
    & export LDFLAGS="-L/usr/local/lib"
    $ export LIBS="-lm -licuuc -licutu -licuio -licui18n -licudata"
    $ ../configure --enable-shared
    $ make
    $ sudo make install


