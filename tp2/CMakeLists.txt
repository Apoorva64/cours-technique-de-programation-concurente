cmake_minimum_required(VERSION 3.30)
project(tp2 C)

set(CMAKE_C_STANDARD 99)

add_executable(tp2
        stack_mutex.c)
#gcc $1 -g -Wall -O3 -msse3 -ftree-vectorize -ftree-vectorizer-verbose=0 \
#-finline-functions -lpthread -march=native -mtune=native \
#-DUSE_CPUS=$USE_CPUS -DN_THREADS=$N_THREADS -DUSE_CPUS=$USE_CPUS \
#-D_GNU_SOURCE -pipe -lm

# Add compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -O3 -msse3 -ftree-vectorize -ftree-vectorizer-verbose=0 -finline-functions -lpthread -march=native -mtune=native -pipe -lm")