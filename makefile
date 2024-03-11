
default: RunTest

DeepClean: Clean
	rm -rf RooUnfold

Clean:
	rm -f Output/*root
	rm -f Plots/*pdf

SetupPrerequisites: SetupRooUnfoldCMake

SetupRooUnfoldMake:
	git clone ssh://git@gitlab.cern.ch:7999/RooUnfold/RooUnfold.git
	(cd RooUnfold/ && make -j4)

SetupRooUnfoldCMake:
	git clone ssh://git@gitlab.cern.ch:7999/RooUnfold/RooUnfold.git
	(cd RooUnfold/ && mkdir -p build && cd build/ && cmake .. && make -j4)

# If you use the make version
# Execute: Unfold1D.cpp
# 	g++ Unfold1D.cpp -o Execute \
# 		-IRooUnfold/src -LRooUnfold/ -lRooUnfold \
# 		`root-config --cflags --glibs`

# If you use the CMake version
Execute: Unfold1D.cpp
	g++ Unfold1D.cpp -o Execute \
		-I/Users/Molly/Desktop/Unfolding/RooUnfold/build/ -L/Users/Molly/Desktop/Unfolding/RooUnfold/build/ -lRooUnfold \
		`root-config --cflags --glibs`

ExecuteScaled: Unfold1DScaled.cpp
	g++ Unfold1DScaled.cpp -o ExecuteScaled \
		-I/Users/Molly/Desktop/Unfolding/RooUnfold/build/ -L/Users/Molly/Desktop/Unfolding/RooUnfold/build/ -lRooUnfold \
		`root-config --cflags --glibs`

ExecuteMSE: Unfold1DMSE.cpp
	g++ Unfold1DMSE.cpp -o ExecuteMSE \
		-I/Users/Molly/Desktop/Unfolding/RooUnfold/build/ -L/Users/Molly/Desktop/Unfolding/RooUnfold/build/ -lRooUnfold \
		`root-config --cflags --glibs`

RunTest: Execute ExecuteScaled ExecuteMSE