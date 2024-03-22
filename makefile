
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
Execute: Unfold1D.cpp
	g++ Unfold1D.cpp -o Execute \
		-I/home/submit/mitay/physics/PhotonJet2018/Unfolding/Unfolding/RooUnfold/src -L/home/submit/mitay/physics/PhotonJet2018/Unfolding/Unfolding/RooUnfold/ -lRooUnfold \
		`root-config --cflags --glibs`

ExecuteMSE: Unfold1DMSE.cpp
	g++ Unfold1DMSE.cpp -o ExecuteMSE \
		-I/home/submit/mitay/physics/PhotonJet2018/Unfolding/Unfolding/RooUnfold/src -L/home/submit/mitay/physics/PhotonJet2018/Unfolding/Unfolding/RooUnfold/ -lRooUnfold \
		`root-config --cflags --glibs`

ExecuteMSELight: Unfold1DMSELight.cpp
	g++ Unfold1DMSELight.cpp -o ExecuteMSELight \
		-I/home/submit/mitay/physics/PhotonJet2018/Unfolding/Unfolding/RooUnfold/src -L/home/submit/mitay/physics/PhotonJet2018/Unfolding/Unfolding/RooUnfold/ -lRooUnfold \
		`root-config --cflags --glibs`

ExecuteErrors: Unfold1DErrors.cpp
	g++ Unfold1DErrors.cpp -o ExecuteErrors \
		-I/home/submit/mitay/physics/PhotonJet2018/Unfolding/Unfolding/RooUnfold/src -L/home/submit/mitay/physics/PhotonJet2018/Unfolding/Unfolding/RooUnfold/ -lRooUnfold \
		`root-config --cflags --glibs`

# If you use the CMake version
# Execute: Unfold1D.cpp
#	g++ Unfold1D.cpp -o Execute \
#		-I/Users/Molly/Desktop/Unfolding/RooUnfold/build/ -L/Users/Molly/Desktop/Unfolding/RooUnfold/build/ -lRooUnfold \
# 		`root-config --cflags --glibs`

# ExecuteScaled: Unfold1DScaled.cpp
#	g++ Unfold1DScaled.cpp -o ExecuteScaled \
#		-I/Users/Molly/Desktop/Unfolding/RooUnfold/build/ -L/Users/Molly/Desktop/Unfolding/RooUnfold/build/ -lRooUnfold \
#		`root-config --cflags --glibs`

# ExecuteMSE: Unfold1DMSE.cpp
#	g++ Unfold1DMSE.cpp -o ExecuteMSE \
#		-I/Users/Molly/Desktop/Unfolding/RooUnfold/build/ -L/Users/Molly/Desktop/Unfolding/RooUnfold/build/ -lRooUnfold \
#		`root-config --cflags --glibs`

RunTest: Execute ExecuteMSE ExecuteMSELight ExecuteErrors
