find . -regex '.*\.\(c\|cpp\|h\)' -exec sed -i "s/#pragma omp/\\/\\/#pragma omp/g" {} \;
find . -regex '.*\.\(c\|cpp\|h\)' -exec clang-format -style=file -i {} \;
find . -regex '.*\.\(c\|cpp\|h\)' -exec sed -i "s/\\/\\/ *#pragma omp/#pragma omp/g" {} \;
