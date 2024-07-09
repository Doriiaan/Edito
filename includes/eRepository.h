

typedef struct eRepository {

	unsigned int n_files;
	
	unsigned int n_repos;

	eFile **files;
	
	struct eRepository **repos;
	
	char *reponame;

} eRepository;
