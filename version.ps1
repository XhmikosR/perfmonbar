<#
.Description
Original version.sh header:
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
#>

# This is the last svn changeset, the number and hash can be automatically
# calculated, but it is slow to do that. So it is better to have it hardcoded.
$svnrev=55
$svnhash="688c3dd699fda7a256dca048466c1902b2c32d6e"

$versionfile="./src/Version.h"

function TryCall($cmdLine) {
    try {
        & ([scriptblock]::Create($cmdLine))
    }
    catch {
        #eat the exception
    }
    return $?
}

#If the git command isn't available or we are not inside a git repo use hardcoded values
if (-not (TryCall('git rev-parse --git-dir 2>&1 | Out-Null'))) {
  $hash = '0000000'
  $ver = 0
} else {
  # Get the current branch name
  $branch = & git symbolic-ref -q HEAD 2>&1
  if ($?) {
    $branch = $branch -replace '^refs/heads/',''
  } else {
    $branch = "no branch"
  }

  # If we are on the main branch
  if ("$branch" -eq "main") {
    $base = "HEAD"
    $version_info = ""
  # If we are on another branch that isn't main, we want extra info like on
  # which commit from main it is based on and what its hash is. This assumes we
  # won't ever branch from a changeset from before the move to git
  } else {
    # Get where the branch is based on main
    $base = & git merge-base main HEAD
    $base_ver = & git rev-list --count "$svnhash..$base"
    $base_ver = [int]$base_ver + $svnrev

    $version_info = "#define BRANCH _T(`"$branch`")`n"
    $ver_full = " ($branch) (main@$base_ver)"
  }

  # Count how many changesets we have since the last svn changeset
  $ver = & git rev-list --count "$svnhash..HEAD"
  # Now add it with to last svn revision number
  $ver = [int]$ver + $svnrev

  # Get the abbreviated hash of the current changeset
  $hash = & git rev-parse --short HEAD
}

$ver_full = "_T(`"$ver ($hash)$ver_full`")"

$version_info += "#define VER_HASH L`"$hash`"`n"
$version_info += "#define VER_NUM 1,0,5,$ver`n"
$version_info += "#define VER_STR `"1.0.5.$ver`"`n"

if ($null -ne $branch) {
  Write-Output "On branch: $branch"
}
Write-Output "Hash:      $hash"
if (($null -ne $branch) -and (-not (TryCall('git diff-index --quiet HEAD')))) {
  Write-Output "Revision:  $ver (Local modifications found)"
} else {
  Write-Output "Revision:  $ver"
}
if ($branch -ne "main") {
  Write-Output "Mergebase: main@$base_ver ($($base.substring(0,7)))"
}

# Update Version.h if it does not exist, or if version information  was changed.
if ((-not (Test-Path($versionfile))) -or ($version_info -ne (Get-Content $versionfile -Raw))) {
  # Write the version information to Version.h
  Set-Content -Path $versionfile -Encoding ascii -NoNewline $version_info
}
