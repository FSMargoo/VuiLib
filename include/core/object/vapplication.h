/*
 * File name	: vapplicaiton.h
 * Author		: Margoo
 * Date			: 11/21/2022
 * Description
 * : The application class VApplication's definition
 */
#pragma once

#include "../uibasic/vtimer.h"
#include "../uibasic/vuitheme.h"
#include "../widget/vmessage.h"
#include "vuiobject.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{
/*
 * VApplication class:
 *  Description : This class is using to describe the application in vuilib
 *  Tips : The
 * VApplicaiton should only be created as once in one single
 *                vuilib project
 */
class VApplication : public VUIObject
{
protected:
	/*
   * GetApplicationTheme override function:
   *     Description : Get the application theme list
   */
	std::vector<VBasicUITheme *> GetApplicationTheme() override;

private:
	/*
   * PatchEvent function:
   *     Description : Patch a event from event stack (if there is any event
   *
	 * exists)
   */
	static VMessage *PatchEvent();
	/*
   * ProcessEvent function:
   *     Description : Send the event to each child objects
   */
	void ProcessEvent(VMessage *PatchedMessage);

public:
	/*
   * Exec function:
   *     Description : Join the main loop
   */
	int Exec();

public:
	/*
   * SetTheme function:
   *     Description : Set the theme as the specified theme
   */
	void SetTheme(VBasicUITheme *Theme);

private:
	/*
   * InitTheme function:
   *     Description : Initialize native theme
   */
	void InitTheme();

public:
	/*
   * IsApplication override function:
   *     Description : To mark this object is a application object
 */
	bool IsApplication() override;

public:
	VApplication();
	~VApplication();

private:
	std::vector<VBasicUITheme *> ThemeList;
};
} // namespace Core

VLIB_END_NAMESPACE