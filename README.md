# CacheSimulator

Steps to run this simulator:

  For Windows:
  
    - Rename "Makefile.windows" to "Makefile"
    - In command window, type:
      - make [This will create all the object files and the executable sim_cache file]
      - sim_cache <BLOCKSIZE> <L1_SIZE> <L1_ASSOC> <L2_SIZE> <L2_ASSOC> <REPLACEMENT_POLICY> <INCLUSION_PROPERTY> <trace_file>
      - make clean [This will delete all the object files and also the executable sim_cache file]
  
  For Linux or Unix:
  
    - Rename "Makefile.unix" to "Makefile"
    - In command window, type:
      - make [This will create all the object files and the executable sim_cache file]
      - ./sim_cache <BLOCKSIZE> <L1_SIZE> <L1_ASSOC> <L2_SIZE> <L2_ASSOC> <REPLACEMENT_POLICY> <INCLUSION_PROPERTY> <trace_file>
      - make clean [This will delete all the object files and also the executable sim_cache file]
 
