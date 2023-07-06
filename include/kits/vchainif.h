#pragma once

namespace VKits
{
typedef class VChainIfJudgement
{
public:
	VChainIfJudgement(const bool &Invalid);

public:
	VChainIfJudgement If(const bool &Expression);
	bool			  Judge(const bool &Expression);

private:
	bool False;
} VIf;
} // namespace VKits
