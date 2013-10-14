#include "mdhim.h"
#include "data_store.h"

int mdhim_leveldb_open(void **dbh, void **dbs, char *path, int flags, 
		       struct mdhim_store_opts_t *mstore_opts);
int mdhim_leveldb_put(void *dbh, void *key, int key_len, void *data, int32_t data_len, 
		      struct mdhim_store_opts_t *mstore_opts);
int mdhim_leveldb_get(void *dbh, void *key, int key_len, void **data, int32_t *data_len, 
		      struct mdhim_store_opts_t *mstore_opts);
int mdhim_leveldb_get_next(void *dbh, void **key, int *key_len, 
			   void **data, int32_t *data_len, void **iterator,
			   struct mdhim_store_opts_t *mstore_opts);
int mdhim_leveldb_get_prev(void *dbh, void **key, int *key_len, 
			   void **data, int32_t *data_len, void **iterator,
			   struct mdhim_store_opts_t *mstore_opts);
int mdhim_leveldb_close(void *dbh, void *dbs, struct mdhim_store_opts_t *mstore_opts);
int mdhim_leveldb_del(void *dbh, void *key, int key_len, 
		      struct mdhim_store_opts_t *mstore_opts);
int mdhim_leveldb_iter_free(void **iterator);
int mdhim_leveldb_commit(void *dbh);
