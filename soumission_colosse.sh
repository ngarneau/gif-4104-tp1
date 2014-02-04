#PBS -A colosse-users
#PBS -l walltime=600
#PBS -l nodes=1:ppn=8

module load compilers/gcc/4.8.0 tools/cmake/2.8.10.2
cd "${PBS_O_WORKDIR}"

./tp1 1000000000
./tp1 1000000000 2
./tp1 1000000000 3
./tp1 1000000000 4
./tp1 1000000000 5
./tp1 1000000000 6
./tp1 1000000000 7
./tp1 1000000000 8