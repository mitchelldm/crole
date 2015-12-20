# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
make
make -C test
rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi
./test/test-crole