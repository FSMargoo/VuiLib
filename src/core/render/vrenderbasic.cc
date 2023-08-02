#include "../../../include/core/render/vrenderbasic.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{
const D2D1_ALPHA_MODE VSurfaceAlphaMode2DXMode(const VSurfaceAlphaMode &AlphaMode) VRENDER_HELPER
{
	/*
	 * Check the value of AlphaMode
	 */
	VLIB_CHECK_REPORT(!(AlphaMode >= 0 && AlphaMode <= 3),
					  L"A invalid value that can't be convert into D2D1_ALPHA_MODE");

	return static_cast<D2D1_ALPHA_MODE>(AlphaMode);
}
} // namespace Core

VLIB_END_NAMESPACE