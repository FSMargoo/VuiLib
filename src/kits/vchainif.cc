#include "kits/vchainif.h"

namespace VKits
{
VChainIfJudgement::VChainIfJudgement(const bool &Invalid)
{
	False = Invalid;
}
VChainIfJudgement VChainIfJudgement::If(const bool &Expression)
{
	if (Expression && False)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool VChainIfJudgement::Judge(const bool &Expression)
{
	if (False)
	{
		return Expression;
	}
	else
	{
		return false;
	}
}
} // namespace VKits
