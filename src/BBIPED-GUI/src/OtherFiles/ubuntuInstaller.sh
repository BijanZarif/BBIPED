#! /bin/sh

####  Fixup $APPNAME.desktop.
APPNAME=BBIPED_platform
ICONNAME=logo/BBiped6.png

cd $(dirname "$0")
APPDIR="$PWD"
EXEC="$APPDIR/$APPNAME"
ICON="$APPDIR/$ICONNAME"
Path="$APPDIR/"

sed -i -e "s@^Icon=.*@Icon=$ICON@" \
    -e "s@^Exec.*@Exec=$EXEC@"  \
    -e "s@^Path.*@Path=$Path@" "$APPNAME.desktop"


###### Checking if some packages are already installed
# Libqwt
#PKG_OK=$(dpkg-query -W --showformat='${Status}\n' libqwt6|grep "install ok installed")
#echo Checking for libqwt6: $PKG_OK
#if [ "" == "$PKG_OK" ]; then
#  echo "No libqwt6. Setting up libqwt6."
#  sudo apt-get install libqwt6
#fi





