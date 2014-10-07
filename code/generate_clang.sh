dir=`pwd`
echo $dir

files=`find $dir -name "*.h"`
echo "" > .clang
for file in $files
do
echo $file >> $dir/.clang
done

folders=`ls -d $dir/*/`
for folder in $folders
do
    cp $dir/.clang $folder/
done


