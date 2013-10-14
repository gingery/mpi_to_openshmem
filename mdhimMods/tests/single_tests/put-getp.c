#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include "mdhim.h"

int main(int argc, char **argv) {
	int ret;
	int provided = 0;
	struct mdhim_t *md;
	int key;
	int value;
	struct mdhim_rm_t *rm;
	struct mdhim_getrm_t *grm;
	int i;
	int keys_per_rank = 5;
	char     *db_path = "./";
	char     *db_name = "mdhimTstDB-";
	int      dbug = MLOG_DBG;
	db_options_t *db_opts; // Local variable for db create options to be passed
	int db_type = 2; //UNQLITE=1, LEVELDB=2 (data_store.h) 

	// Create options for DB initialization
	db_opts = db_options_init();
	db_options_set_path(db_opts, db_path);
	db_options_set_name(db_opts, db_name);
	db_options_set_type(db_opts, db_type);
	db_options_set_key_type(db_opts, MDHIM_INT_KEY);
	db_options_set_debug_level(db_opts, dbug);

	ret = MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
	if (ret != MPI_SUCCESS) {
		printf("Error initializing MPI with threads\n");
		exit(1);
	}

	if (provided != MPI_THREAD_MULTIPLE) {
                printf("Not able to enable MPI_THREAD_MULTIPLE mode\n");
                exit(1);
        }

	md = mdhimInit(MPI_COMM_WORLD, db_opts);
	if (!md) {
		printf("Error initializing MDHIM\n");
		exit(1);
	}	

	//Put the keys and values
	for (i = 0; i < keys_per_rank; i++) {
		key = keys_per_rank * md->mdhim_rank + i;
		value = md->mdhim_rank + i;
		rm = mdhimPut(md, &key, sizeof(key), 
			      &value, sizeof(value));
		if (!rm || rm->error) {
			printf("Error inserting key/value into MDHIM\n");
		} else {
			printf("Rank: %d put key: %d with value: %d\n", md->mdhim_rank, key, value);
		}
	}

	//Commit the database
	ret = mdhimCommit(md);
	if (ret != MDHIM_SUCCESS) {
		printf("Error committing MDHIM database\n");
	} else {
		printf("Committed MDHIM database\n");
	}

	//Get the stats
	ret = mdhimStatFlush(md);
	if (ret != MDHIM_SUCCESS) {
		printf("Error getting stats\n");
	} else {
		printf("Got stats\n");
	}

	//Get the values using get_prev
	for (i = keys_per_rank; i > 0; i--) {
		value = 0;
		key = keys_per_rank * md->mdhim_rank + i;
		grm = mdhimGet(md, &key, sizeof(int), MDHIM_GET_PREV);				
		if (!grm || grm->error) {
			printf("Error getting value for key: %d from MDHIM\n", key);
		} else if (grm->key && grm->value) {
			printf("Rank: %d successfully got key: %d with value: %d from MDHIM\n", 
			       md->mdhim_rank,
			       *((int *) grm->key),
			       *((int *) grm->value));
		}
	}

	ret = mdhimClose(md);
	if (ret != MDHIM_SUCCESS) {
		printf("Error closing MDHIM\n");
	}

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();

	return 0;
}
