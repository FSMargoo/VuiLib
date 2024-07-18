/*
 * Copyright (c) 2023~Now Margoo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * \file main.cpp
 * \brief The toolchain of VUILib for git commit
 */

#include <include/cmdparser.hpp>
#include <third/SimpleJson/sJSON.h>

#include <Windows.h>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <map>
#include <shellapi.h>
#include <string>

#define README                                                                                                         \
	R"(<div align="center">

<img src="./readme/icon.svg" width="300">

![GitHub License](https://img.shields.io/github/license/FSMargoo/VUILib) ![GitHub Created At](https://img.shields.io/github/created-at/FSMargoo/VuiLib)

<h3>Auto Commit Status</h3>

![Static Badge](https://img.shields.io/badge/Unit%20Test-{}-{})
![Static Badge](https://img.shields.io/badge/Last%20Commit-{}-blue)
![Static Badge](https://img.shields.io/badge/Last%20Commit%20Date-{}-purple)

<h3 align="center">VUILib3.x</h3>
A cross-platform UI Library born for flexible and efficient. Build with:

<br>

<img src="https://skillicons.dev/icons?i=cpp,cmake,clion,figma" />

<br>

VUILib 3.x is still under developing, please wait with our first dev release!

</div>

## Acknowledgements

Thanks for $JetBrains$'s Licenses for Open Source Development

<div align="center">

<img src="https://resources.jetbrains.com/storage/products/company/brand/logos/jetbrains.png" alt="JetBrains logo." width="300">

</div>

Thanks for $mohabouje$'s project $WinToast$.

Thanks for $FlorianRappl$'s project $CmdParser$.

Thanks for $Google$'s project $Skia$

Thanks for $GLFW$ project

## Third-party Libraries List

<div align="center">

[mohabouje-WinToast](https://github.com/mohabouje/WinToast)

[FlorianRappl-CmdParser](https://github.com/FlorianRappl/CmdParser)

[Margoo-SimpleJson](https://github.com/FSMargoo/SimpleJson)

[Skia](skia.org)

[GLFW](https://github.com/GLFW/glfw)

</div>)"

std::map<std::string, std::string> AbbreviationToEmoji	  = {{"db", ":bug:"},
															 {"up", ":zap:"},
															 {"nf", ":sparkles:"},
															 {"dc", ":books:"},
															 {"vr", ":bookmark:"},
															 {"at", ":white_check_mark:"},
															 {"ts", ":rotating_light:"},
															 {"pf", ":racehorse:"},
															 {"rc", ":hammer:"},
															 {"rm", ":fire:"},
															 {"wi", ":construction:"},
															 {"cf", ":wrench:"},
															 {"ni", ":hankey:"},
															 {"bc", ":boom:"},
															 {"cr", ":ok_hand:"},
															 {"tp", ":heavy_check_mark:"}};
std::map<std::string, std::string> AbbreviationToFullName = {{"db", "debug"},
															 {"up", "updates"},
															 {"nf", "new feature"},
															 {"dc", "document"},
															 {"vr", "version release"},
															 {"at", "add test"},
															 {"ts", "testing"},
															 {"pf", "performance"},
															 {"rc", "refactor code"},
															 {"rm", "removing code/files"},
															 {"wi", "working in progress"},
															 {"cf", "configure files"},
															 {"ni", "bad code / need improve"},
															 {"bc", "breaking change"},
															 {"cr", "code review"},
															 {"tp", "make a test pass"}};

/**
 * Generate a commit id for commit
 * @param commitType db : debug (:bug:)
 * up : updates (:zap:)
 * nf : new feature (:sparkles:)
 * dc : document (:books:)
 * vr : version release(:bookmark:)
 * at : add test (:white_check_mark:)
 * ts : testing (:rotating_light:)
 * pf : performance (:racehorse:)
 * rc : refactor code (:hammer:)
 * rm : removing code/files (:fire:)
 * wi : working in progress (:construction:)
 * cf : configure files (:wrench:)
 * ni : bad code / need improve (:hankey:)
 * bc : breaking change (:boom:)
 * cr : code review (:ok_hand:)
 * tp : make a test pass (:heavy_check_mark:)
 *
 * @return The commit id in string format
 */
std::string GenerateCommitId(const std::string &commitType) {
	std::stringstream format;
	std::string		  date;
	format << std::hex << time(0);
	format >> date;

	if (commitType != "rm" || commitType != "rc" || commitType != "cr") {
		return commitType + ":" + date;
	} else {
		return "@" + commitType + ":" + date;
	}
}

/**
 * Config the CLI command line parser
 * @param parser The target parser
 */
void ConfigureParser(cli::Parser &parser) {
	parser.set_required<std::string>("t", "type", R"(The type of commit:
	db : debug (:bug:)
	up : updates (:zap:)
	nf : new feature (:sparkles:)
	dc : document (:books:)
	vr : version release(:bookmark:)
	at : add test (:white_check_mark:)
	ts : testing (:rotating_light:)
	pf : performance (:racehorse:)
	rc : refactor code (:hammer:)
	rm : removing code/files (:fire:)
	wi : working in progress (:construction:)
	cf : configure files (:wrench:)
	ni : bad code / need improve (:hankey:)
	bc : breaking change (:boom:)
	cr : code review (:ok_hand:)
	tp : make a test pass (:heavy_check_mark:))");
	parser.set_required<std::string>("m", "message", "The commit message");
	parser.set_optional<std::string>("b", "branch", "");
	parser.set_optional<std::string>("r", "repository", "v");
}

int main(int argc, const char **argv) {
	cli::Parser parser(argc, argv);
	ConfigureParser(parser);
	parser.run_and_exit_if_error();

	auto type		= parser.get<std::string>("t");
	auto message	= parser.get<std::string>("m");
	auto branch		= parser.get<std::string>("b");
	auto repository = parser.get<std::string>("r");
	branch			= branch.empty() ? "main" : branch;

	// Check for abbreviation
	if (AbbreviationToFullName.find(type) == AbbreviationToFullName.end()) {
		printf("%s", std::format("Invalid commit type {}!", type).c_str());
		exit(-1);
	}

	auto commitId = GenerateCommitId(type);

	printf("%s", std::format("Running on the current repository with branch {}.\nCommit type {}({}), commit id {}.\n",
							 branch, type, AbbreviationToFullName[type], commitId)
					 .c_str());

	printf("Running unit test...\n");

	SHELLEXECUTEINFO unitTest = {0};
	unitTest.cbSize			  = sizeof(SHELLEXECUTEINFO);
	unitTest.fMask			  = SEE_MASK_NOCLOSEPROCESS;
	unitTest.hwnd			  = nullptr;
	unitTest.lpVerb			  = nullptr;
	unitTest.lpFile			  = TEXT("cmake-build-debug\\UnitTest.exe");
	unitTest.lpParameters	  = TEXT("");
	unitTest.lpDirectory	  = nullptr;
	unitTest.nShow			  = SW_SHOWMINIMIZED;
	unitTest.hInstApp		  = nullptr;
	ShellExecuteEx(&unitTest);

	WaitForSingleObject(unitTest.hProcess, INFINITE);

	printf("Unit test has all done!\nReading vtest.json......\n");

	std::ifstream stream;
	stream.open("./vtest.json");
	bool flag = false;
	if (stream.is_open()) {
		std::string jsonContext;
		std::string line;
		while (!stream.eof()) {
			std::getline(stream, line);
			jsonContext += line;
		}

		sJSONParser parser(jsonContext);
		auto		root = parser.Parse();
		for (auto Object = root["tests"].ArrayBegin(); Object != root["tests"].ArrayEnd(); ++Object) {
			auto ObjectExpand = sJSONElementFinder((*Object)->To<sJSONObject *>());
			std::string testID;
			for (auto Instance : ObjectExpand) {
				if (sJSONString::Equal(**Instance.second)) {
					testID = (**Instance.second->To<sJSONString *>());
				}

			}
			for (auto Instance : ObjectExpand) {
				if (sJSONInt::Equal(**Instance.second)) {
					if (**Instance.second->To<sJSONInt *>() == 0) {
						printf("%s\n", std::format("WARNING : Test {} failed! [×]", testID).c_str());
						flag = true;
					} else {
						printf("%s\n", std::format("Test {} Passed! [√]", testID).c_str());
					}
				}
			}
		}

		stream.close();
	} else {
		printf("Failed to read the testing result file!");
		exit(-1);
	}

	auto const time = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());

	auto readmeContext =
		std::format(README, flag == true ? "Failed" : "Pass", flag == true ? "red" : "green", commitId, std::format("{:%Y/%m/%d/%X}", time));
	std::ofstream readme("./README.md");
	readme << readmeContext;
	readme.close();

	printf("README file has been written!\n");
	printf("Git operation!\n");

	printf(">>> git add *\n");
	system("git add *");
	printf(">>> %s\n", std::format("git commit -m \"{}{}:{}\"", AbbreviationToEmoji[type], commitId, message).c_str());
	system(std::format("git commit -m \"{}{}:{}\"", AbbreviationToEmoji[type], commitId, message).c_str());
	printf(">>> %s\n", std::format("git push {} main", repository).c_str());
	system(std::format("git push {} main", repository).c_str());

	return 0;
}