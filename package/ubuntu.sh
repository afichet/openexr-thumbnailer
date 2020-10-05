#!/bin/sh
# Inspired from RetroShare packaging script

MAJOR_VERSION=1
MINOR_VERSION=2
PKG_NAME="openexr-thumbnailer"
REPO=git@github.com:afichet/openexr-thumbnailer.git
FOLDER=openexr-thumbnailer
dist="bionic focal groovy"
ppa_addr="ppa:alban-f/openexr-thumbnailer"
version_number="${MAJOR_VERSION}"'.'"${MINOR_VERSION}"
package_dir="${PKG_NAME}"'-'"${version_number}"

echo "Building package for "${package_dir}

if test -d "${package}" ; then
    rm -rf ${package_dir}
fi

date=`git log --pretty=format:"%ai" | head -1 | cut -d\  -f1 | sed -e s/-//g`
time=`git log --pretty=format:"%aD" | head -1 | cut -d\  -f5 | sed -e s/://g`
hhsh=`git log --pretty=format:"%H"  | head -1 | cut -c1-8`
rev=${date}.${hhsh}

change=`git log --pretty="%h %an %aD %s"`

mkdir ${package_dir}
cd ${package_dir}

# Creating source tarball
git clone "${REPO}" "${FOLDER}"
cd "${FOLDER}"
rm -rf .git
cd ..
tar cvzf "${PKG_NAME}_${version_number}.orig.tar.gz" "${FOLDER}"

# For each distribution, generating a changelog
for i in ${dist}; do
    cd "${FOLDER}"
    cp -r ../../debian .
    sed -e s/XXXXXX/"${rev}"/g -e s/YYYYYY/"${i}"/g -e s/ZZZZZZ/"${version_number}"/g ../../debian/changelog > debian/changelog
    debuild -S
    cd ..
    dput ${ppa_addr} "${PKG_NAME}_${version_number}-1.${rev}~${i}_source.changes"
done
