ln -s /afs/cern.ch/atlas/offline/external/Eigen/3.1.3/x86_64-slc6-gcc48-opt/Eigen 
echo "export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase" >> $HOME/.bashrc
echo "alias setupATLAS='source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh'" >> $HOME/.bashrc
echo "setupATLAS --quiet" >> $HOME/.bashrc
echo "asetup 19.0.0" >> $HOME/.bashrc
