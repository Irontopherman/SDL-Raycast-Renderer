#rm build/$(find build/ -type f -executable -exec file {} \; | grep -Ei 'ELF|Mach-O' | tail -n1 |cut -d: -f1 | xargs basename)
rm -r build
mkdir build
cd build
cmake ..
make
cd ..
#BIN_NAME=$(find build/ -type f -executable -exec file {} \; | grep -Ei 'ELF|Mach-O' | tail -n1 |cut -d: -f1 | xargs basename)
#cd build
#./"$BIN_NAME"

