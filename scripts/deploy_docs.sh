#!/bin/bash
# Deploy docs to GitHub pages

scriptDir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
projDir="${scriptDir}/../"

pushd ${projDir} > /dev/null

# Get remote associated with GitHub
ghRemote="$(git remote -v | grep github | grep push | cut -f1)"

if [ "x${ghRemote}" == "x" ]; then
  echo "No GitHub remote found!"
  popd > /dev/null
  exit -1
fi

echo "Using GitHub remote ${ghRemote}"
mkdocs gh-deploy -r ${ghRemote}

popd > /dev/null
