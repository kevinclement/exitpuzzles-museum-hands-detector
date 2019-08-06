import os
git_hash = os.popen('git --no-pager describe --tags --always --dirty').read().strip()
git_date = os.popen('git --no-pager show --date=format:"%Y-%m-%d %H:%M:%S" --format="%ad" --name-only').read().partition('\n')[0].strip()

print("######### generate_git_header #############")

print("hash: '{0}'".format(git_hash))
print("date: '{0}'".format(git_date))