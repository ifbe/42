echo "#################################vendor#############################"
dmidecode | grep "Product Name" 
dmidecode | grep -i 'serial number'
echo ""

echo "###################################cpu#############################"
uname -m
cat /proc/cpuinfo | grep name | cut -f2 -d: | uniq -c
echo ""

echo "##################################system#############################"
uname -a
echo ""

echo "##################################gcc################################"
gcc --version
echo ""

echo "##################################other##############################"
cat /proc/bus/input/handlers
echo ""
