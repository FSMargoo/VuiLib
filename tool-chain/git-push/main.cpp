#include "fmt/vfmt.h"
#include "vml/vmlparser.h"
#include <iostream>
#include <map>

#pragma comment(lib, "vuilib.lib")

int main()
{
	using namespace Core;

	VML::VMLParser		 Parser(VStr("./workflow.xml"), VML::VMLParserParseMode::FromFile);
	VML::VMLParserResult Result = Parser.ParseVML();
	if (Result.ParserStatus != VML::VMLParserStatus::Ok)
	{
		VFmt::Print(VStr("Failed to parse \"workflow.xml\"!\n"));

		return -1;
	}
	auto StartupNode = Result.Nodes["workflow"].ChildrenNodes["startup"];
	if (VKits::VIf(StartupNode.PropertyExsit(VStr("text")))
			.Judge(StartupNode.GetProperty(VStr("text")).PropertyType == VML::VMLPropertyType::StringValue))
	{
		VFmt::Print("From Project Configure : {}\n", StartupNode.GetProperty(VStr("text")).PropertyAsString);
	}
	auto SysNode = Result.Nodes["workflow"].ChildrenNodes["sys"];
	if (VKits::VIf(SysNode.PropertyExsit(VStr("command")))
			.Judge(SysNode.GetProperty(VStr("command")).PropertyType == VML::VMLPropertyType::StringValue))
	{
		bstr_t Convertor(SysNode.GetProperty(VStr("command")).PropertyAsString.CStyleString());
		system((char *)Convertor);

		VFmt::Print("Excuse Project Command <{}>\n", SysNode.GetProperty(VStr("command")).PropertyAsString);
	}
	auto	GitRepo = Result.Nodes["workflow"].ChildrenNodes["git-repository"];
	VString RepoName;
	VString RepoBranch;
	if (VKits::VIf(GitRepo.PropertyExsit(VStr("repos-name")) && GitRepo.PropertyExsit(VStr("branch")))
			.Judge(GitRepo.GetProperty(VStr("repos-name")).PropertyType == VML::VMLPropertyType::StringValue &&
				   GitRepo.GetProperty(VStr("branch")).PropertyType == VML::VMLPropertyType::StringValue))
	{
		RepoName   = GitRepo.GetProperty(VStr("repos-name")).PropertyAsString;
		RepoBranch = GitRepo.GetProperty(VStr("branch")).PropertyAsString;

		VFmt::Print("Git Repository at <{}>[{}]\n", GitRepo.GetProperty(VStr("repos-name")).PropertyAsString,
					GitRepo.GetProperty(VStr("branch")).PropertyAsString);
	}
	else
	{
		VFmt::Print("Git-Autor failed : Unable to find the correct \"git-repository\" node in \"workflow.xml\".");

		return -1;
	}

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

	GitCommand = VFmt::Format("git push {} {}", RepoName, RepoBranch);
	Proxy	   = GitCommand.CStyleString();
	system((char *)Proxy);

	return 0;
}