#include "fmt/vfmt.h"
#include <iostream>
#include <map>

#pragma comment(lib, "vuilib.lib")

int main()
{
	using namespace Core;

	system("auto-format\n");

	std::map<int, VString> CommentMapping = {{0, ":new: feat"},
											 {1, ":bug: fix/to"},
											 {2, ":book: docs"},
											 {3, ":art: style"},
											 {4, ":hammer: refactor"},
											 {5, ":rocket: pref"},
											 {6, ":bookmark: test"},
											 {7, ":wrench: chore"},
											 {8, ":twisted_rightwards_arrows: revert"},
											 {9, ":construction: working"}};

	VFmt::Print("{}\n", VString().Fill('-', 40));
	for (auto &Mapping : CommentMapping)
	{
		VFmt::Print("[{}] {}\n", Mapping.first, Mapping.second);
	}
	VFmt::Print("{}\nCommit Type : ", VString().Fill('-', 40));

	int Selection;
	std::cin >> Selection;

	std::string Scope;
	VFmt::Print("Scope Info : ");
	std::cin >> Scope;

	VString ScopeString = VString::FromString(Scope);

	std::string Subject;
	VFmt::Print("Subject Info : ");
	std::cin.ignore();
	std::getline(std::cin, Subject);

	VString SubjectString = VString::FromString(Subject);

	VString GitCommand = VFmt::Format("git add *");

	bstr_t Proxy(GitCommand.CStyleString());
	system((char *)Proxy);
	VFmt::Print("cmd.exe \"{}\"\n", GitCommand);

	GitCommand = VFmt::Format("git commit -m \"{}({}) : {}\"", CommentMapping[Selection], Scope, Subject);
	VFmt::Print("cmd.exe \"{}\"\n", GitCommand);
	Proxy = GitCommand.CStyleString();
	system((char *)Proxy);

	GitCommand = VFmt::Format("git push vlib main");
	Proxy	   = GitCommand.CStyleString();
	system((char *)Proxy);

	return 0;
}