
# Standalone unfolding code

## Setting up

Everything depends on `ROOT` so make sure it is available before going into other steps

Once you check-out/download the code, first we need to get `TUnfold` and `RooUnfold`.  They can be done with

```
make SetupRooUnfoldCMake
```

(or `make SetupRooUnfoldMake` if your cmake version is too old and it's not easy to update)

This only needs to be done once!  If things are messed up, we can do

```
make DeepClean
```

to return to the initial state and try again.



## Compiling code

Depending on whether you used the `cmake` or the `make` version to compile RooUnfold, change the `makefile` accordingly.


Do

```
make Execute
```

to compile the main executable.



## Running code

There are a few arguments to pass to the code.  Listed below

| Argument         | type   | default     | note                                                  |
|------------------|--------|-------------|-------------------------------------------------------|
| Input            | string | *required*  | input file name                                       |
| InputName        | string | "HDataReco" | input histogram name                                  |
| ResponseName     | string | "HResponse" | response histogram name                               |
| ResponseTruth    | string | "HMCGen"    | truth distribution (used as prior for example)        |
| ResponseMeasured | string | "HMCReco"   | reco-level MC histogram name                          |
| Output           | string | *required*  | output file name                                      |
| PriorChoice      | string | "Original"  | what kind of prior to use                             |
| DoBayes          | bool   | true        | whether to run BayesUnfold                            |
| DoRepeatedBayes  | bool   | true        | whether do run repeated BayesUnfold                   |
| DoSVD            | bool   | true        | whether to run SVD unfold                             |
| DoInvert         | bool   | true        | whether to run inversion unfold                       |
| DoFit            | bool   | true        | whether to run fitting unfold                         |
| DoFoldNormalize  | bool   | false       | whether to normalize output with folded distribution  |
| DoToyError       | bool   | false       | whether to use toy error (slow) or not                |


There are a few possible choices of prior:

| PriorChoice  | Extra arguments                           | note                                                                        |
|--------------|-------------------------------------------|-----------------------------------------------------------------------------|
| "MC"         | _none_                                    | Use the truth distribution that comes with the file                         |
| "ExternalMC" | ExternalPriorFile                         | use the truth distribution from that file                                   |
| "External"   | ExternalPriorFile, ExternalPriorHistogram | use the truth distribution from that histogram in that file                 |
| "Flat"       | _none_                                    | generates a flat prior to use                                               |
| "Power"      | PriorK                                    | generates a power-law with given power constant                             |
| "Original"   | _none_                                    | do nothing!  equivalent to using projection of the response matrix as prior |




