import os
import datetime

os.system("\"auto-format.bat\"");

CommitType = int(input("0 : feat\n1: fix/to\n2: docs\n3: style\n4: refactor\n5 : perf"
"Commit type() : "))
CommitInfo = input("Commit info : ");
CommitInfo = datetime.datetime.now().strftime('[%Y-%m-%d %H:%M:%S]') + CommitInfo;

print(CommitInfo)