# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
make mode=release
make -C bench
rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi
cd bench/bin
for bench in *.bench*; do
	time ./$bench;
done;
cd ../..