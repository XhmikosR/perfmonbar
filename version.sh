#!/bin/bash
# (C) 2012-2013 see Authors.txt
#
# This file is part of MPC-HC.
#
# MPC-HC is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# MPC-HC is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

# This is the last svn changeset, the number and hash can be automatically
# calculated, but it is slow to do that. So it is better to have it hardcoded.
svnrev=55
svnhash="688c3dd699fda7a256dca048466c1902b2c32d6e"

versionfile="./src/Version.h"

#If the git command isn't available or we are not inside a git repo use hardcoded values
if ! git rev-parse --git-dir > /dev/null 2>&1; then
  hash=0000000
  ver=0
else
  # Get the current branch name
  branch=$(git symbolic-ref -q HEAD) && branch=${branch##refs/heads/} || branch="no branch"
  # If we are on the main branch
  if [[ "$branch" == "main" ]]; then
    base="HEAD"
  # If we are on another branch that isn't main, we want extra info like on
  # which commit from main it is based on and what its hash is. This assumes we
  # won't ever branch from a changeset from before the move to git
  else
    # Get where the branch is based on main
    base=$(git merge-base main HEAD)
    base_ver=$(git rev-list --count $svnhash..$base)
    base_ver=$((base_ver+svnrev))

    version_info="#define BRANCH _T(\"$branch\")"$'\n'
    ver_full=" ($branch) (main@${base_ver:0:7})"
  fi

  # Count how many changesets we have since the last svn changeset
  ver=$(git rev-list --count $svnhash..HEAD)
  # Now add it with to last svn revision number
  ver=$((ver+svnrev))

  # Get the abbreviated hash of the current changeset
  hash=$(git rev-parse --short HEAD)

fi

ver_full="_T(\"$ver ($hash)$ver_full\")"

version_info+="#define VER_HASH L\"$hash\""$'\n'
version_info+="#define VER_NUM 1,0,5,$ver"$'\n'
version_info+="#define VER_STR \"1.0.5.$ver\""

if [[ "$branch" ]]; then
  echo "On branch: $branch"
fi
echo "Hash:      $hash"
if [[ "$branch" ]] && ! git diff-index --quiet HEAD; then
  echo "Revision:  $ver (Local modifications found)"
else
  echo "Revision:  $ver"
fi
if [[ "$branch" ]] && [[ "$branch" != "main" ]]; then
  echo "Mergebase: main@${base_ver} (${base:0:7})"
fi

# Update Version.h if it does not exist, or if version information  was changed.
if [[ ! -f "$versionfile" ]] || [[ "$version_info" != "$(<"$versionfile")" ]]; then
  # Write the version information to Version.h
  echo "$version_info" > "$versionfile"
fi
