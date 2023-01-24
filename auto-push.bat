@echo on
git add *
set /p commit_info="Git commit info: "
git commit -m "%commit_info%"
git push vlib main
pause