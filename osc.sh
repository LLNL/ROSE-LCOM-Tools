
#!/bin/bash

git clone https://github.com/bkungl/AdaExploiting.git testcases/osc/AdaExploiting &

(
  wget https://sourceforge.net/projects/adasl/files/adasl/adasl-1.5.tar.gz -P testcases/osc
  tar -xvzf testcases/osc/adasl-1.5.tar.gz -C testcases/osc
  rm testcases/osc/adasl-1.5.tar.gz
) &

git clone https://github.com/alire-project/alire.git testcases/osc/alire &

(
  wget https://sourceforge.net/projects/booch95/files/booch95/20190412/bc-20190412.tar.bz2 -P testcases/osc
  tar -xvf testcases/osc/bc-20190412.tar.bz2 -C testcases/osc
  rm testcases/osc/bc-20190412.tar.bz2
) &

(
  wget https://sourceforge.net/projects/simplecomponentsforada/files/releases/components_4_67.tgz -P testcases/osc
  mkdir -p testcases/osc/components
  tar -xvzf testcases/osc/components_4_67.tgz -C testcases/osc/components
  rm testcases/osc/components_4_67.tgz
) &

(
  wget https://sourceforge.net/projects/fuzzysetsforada/files/releases/fuzzy_5_15.tgz -P testcases/osc
  mkdir -p testcases/osc/fuzzy
  tar -xvzf testcases/osc/fuzzy_5_15.tgz -C testcases/osc/fuzzy
  rm testcases/osc/fuzzy_5_15.tgz
) &

git clone https://github.com/AdaCore/gnatstudio.git  testcases/osc/gnatstudio &

git clone https://github.com/AdaCore/libadalang-tools.git testcases/osc/libadalang-tools &

(
  wget https://sourceforge.net/projects/linxtris/files/linxtris/0.1a2/linxtris-0.1a2-src.tar.bz2 -P testcases/osc
  tar -xvf testcases/osc/linxtris-0.1a2-src.tar.bz2 -C testcases/osc
  rm testcases/osc/linxtris-0.1a2-src.tar.bz2
) &

git clone https://github.com/janverschelde/PHCpack.git testcases/osc/PHCpack &

(
  wget https://sourceforge.net/projects/png-io/files/png-io/4.6/png_4_6.zip -P testcases/osc
  unzip testcases/osc/png_4_6.zip -d testcases/osc
  rm testcases/osc/png_4_6.zip
)& 

git clone https://github.com/UlrikHjort/SHA-1.git testcases/osc/SHA-1 &

(
  wget https://sourceforge.net/projects/sx-ada-kalinda/files/sx-ada-kalinda/%5BUnnamed%20release%5D/sx-ada-kalinda.zip -P testcases/osc
  unzip testcases/osc/sx-ada-kalinda.zip -d testcases/osc/sx-ada-kalinda
rm testcases/osc/sx-ada-kalinda.zip
) &

wait

echo "All open-source code downloaded into testcases/osc"