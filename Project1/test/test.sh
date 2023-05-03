#!/bin/sh
progTmp="$(mktemp -d)"
sysTmp="$(mktemp -d)"
testName="$1"
cd "$(dirname "$0")/" || exit 1
if [ ! -f "$testName.sh" ]; then
    echo "Test suite not found!"
    exit 1
fi
sh -c 'cd .. && make'
cp -a "${testName}.sh" "${testName}"/* "${progTmp}/"
cp -a "${testName}.sh" "${testName}"/* "${sysTmp}/"
echo "Using test suite ${testName}":
cat "${testName}.sh"
echo "Testing program: "
sh -c "cd \"$progTmp\" && $(pwd)/../mysh ${testName}.sh"
echo
echo "Making system shell sample: "
sh -c "cd \"$sysTmp\" && sh -x \"$sysTmp\"/${testName}.sh"
echo "Making diff..."
while read file || [ -n "$file" ]; do
    if (diff "$progTmp/$file" "$sysTmp/$file"); then
        echo "✅ $file\tTEST PASSED!"
    else
        echo "❌ $file\tTEST FAILED!"
    fi
done < "${testName}.out"
echo "Removing artifacts..."
rm -rf "$progTmp" "$sysTmp"
