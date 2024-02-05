
# file="data/processed/20_USA-road-d.COL.gr"
file="data/processed/1300_USA-road-d.BAY.gr"

# method="static"
# method="heuristic"
method="branch_and_cut"
# method="dualized"
time_limit=600

verbose=2

make release

echo "Running file: $file"
./myprogram "$file" "$method" "$time_limit" "$verbose"

# make clean
