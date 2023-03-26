#pragma warning(disable : 4018)
#pragma warning(disable : 26451)

#include "../../../include/core/control/veditor.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{
void VEditorCaret::CaretSelectionTurnUp(IDWriteTextLayout *TextLayout)
{
	InSelecting = true;

	if (CaretStart == CaretEnd)
	{
		DWRITE_TEXT_METRICS				 TextMetrics;
		std::vector<DWRITE_LINE_METRICS> LineMetrics;

		TextLayout->GetMetrics(&TextMetrics);

		LineMetrics.resize(TextMetrics.lineCount);

		TextLayout->GetLineMetrics(&LineMetrics[0], TextMetrics.lineCount, &TextMetrics.lineCount);

		UINT32 Line				= 0;
		UINT32 LinePosition		= 0;
		UINT32 NextLinePosition = 0;
		UINT32 LineCount		= static_cast<UINT32>(LineMetrics.size());
		for (; Line < LineCount; ++Line)
		{
			LinePosition	 = NextLinePosition;
			NextLinePosition = LinePosition + LineMetrics[Line].length;

			if (NextLinePosition > CaretStart)
			{
				break;
			}
		}

		if (Line > LineCount - 1)
		{
			Line = LineCount - 1;
		}

		if (Line <= 0)
		{
			return;
		}

		--Line;
		LinePosition -= LineMetrics[Line].length;

		DWRITE_HIT_TEST_METRICS HitTestMetrics;

		FLOAT CaretX		  = 0;
		FLOAT CaretY		  = 0;
		FLOAT UnusedParemeter = 0;

		BOOL IsInside	   = FALSE;
		BOOL IsTrailingHit = FALSE;

		TextLayout->HitTestTextPosition(CaretStart, FALSE, &CaretX, &CaretY, &HitTestMetrics);
		TextLayout->HitTestTextPosition(LinePosition, FALSE, &UnusedParemeter, &CaretY, &HitTestMetrics);
		TextLayout->HitTestPoint(CaretX, CaretY, &IsTrailingHit, &IsInside, &HitTestMetrics);

		CaretStart = HitTestMetrics.textPosition;

		if (IsTrailingHit)
		{
			CaretStart = CacheTargetPtr->size();
		}

		InSelecting = true;
		SelectMode	= VEditorCaretSelectMode::Left;

		SetValidCaretRange();

		return;
	}
	else
	{
		switch (SelectMode)
		{
		case VEditorCaretSelectMode::Left: {
			DWRITE_TEXT_METRICS				 TextMetrics;
			std::vector<DWRITE_LINE_METRICS> LineMetrics;

			TextLayout->GetMetrics(&TextMetrics);

			LineMetrics.resize(TextMetrics.lineCount);

			TextLayout->GetLineMetrics(&LineMetrics[0], TextMetrics.lineCount, &TextMetrics.lineCount);

			UINT32 Line				= 0;
			UINT32 LinePosition		= 0;
			UINT32 NextLinePosition = 0;
			UINT32 LineCount		= static_cast<UINT32>(LineMetrics.size());
			for (; Line < LineCount; ++Line)
			{
				LinePosition	 = NextLinePosition;
				NextLinePosition = LinePosition + LineMetrics[Line].length;

				if (NextLinePosition > CaretStart)
				{
					break;
				}
			}

			if (Line > LineCount - 1)
			{
				Line = LineCount - 1;
			}

			if (Line <= 0)
			{
				return;
			}

			--Line;
			LinePosition -= LineMetrics[Line].length;

			DWRITE_HIT_TEST_METRICS HitTestMetrics;

			FLOAT CaretX		  = 0;
			FLOAT CaretY		  = 0;
			FLOAT UnusedParemeter = 0;

			BOOL IsInside	   = FALSE;
			BOOL IsTrailingHit = FALSE;

			TextLayout->HitTestTextPosition(CaretStart, FALSE, &CaretX, &CaretY, &HitTestMetrics);
			TextLayout->HitTestTextPosition(LinePosition, FALSE, &UnusedParemeter, &CaretY, &HitTestMetrics);
			TextLayout->HitTestPoint(CaretX, CaretY, &IsTrailingHit, &IsInside, &HitTestMetrics);

			CaretStart = HitTestMetrics.textPosition;

			if (CaretEnd < CaretStart)
			{
				SelectMode = VEditorCaretSelectMode::Right;
			}

			InSelecting = true;

			SetValidCaretRange();

			return;
		}
		case VEditorCaretSelectMode::Right: {
			DWRITE_TEXT_METRICS				 TextMetrics;
			std::vector<DWRITE_LINE_METRICS> LineMetrics;

			TextLayout->GetMetrics(&TextMetrics);

			LineMetrics.resize(TextMetrics.lineCount);

			TextLayout->GetLineMetrics(&LineMetrics[0], TextMetrics.lineCount, &TextMetrics.lineCount);

			UINT32 Line				= 0;
			UINT32 LinePosition		= 0;
			UINT32 NextLinePosition = 0;
			UINT32 LineCount		= static_cast<UINT32>(LineMetrics.size());
			for (; Line < LineCount; ++Line)
			{
				LinePosition	 = NextLinePosition;
				NextLinePosition = LinePosition + LineMetrics[Line].length;

				if (NextLinePosition > CaretEnd)
				{
					break;
				}
			}

			if (Line > LineCount - 1)
			{
				Line = LineCount - 1;
			}

			if (Line <= 0)
			{
				return;
			}

			--Line;
			LinePosition -= LineMetrics[Line].length;

			DWRITE_HIT_TEST_METRICS HitTestMetrics;

			FLOAT CaretX		  = 0;
			FLOAT CaretY		  = 0;
			FLOAT UnusedParemeter = 0;

			BOOL IsInside	   = FALSE;
			BOOL IsTrailingHit = FALSE;

			TextLayout->HitTestTextPosition(CaretEnd, FALSE, &CaretX, &CaretY, &HitTestMetrics);
			TextLayout->HitTestTextPosition(LinePosition, FALSE, &UnusedParemeter, &CaretY, &HitTestMetrics);
			TextLayout->HitTestPoint(CaretX, CaretY, &IsTrailingHit, &IsInside, &HitTestMetrics);

			CaretEnd = HitTestMetrics.textPosition;

			if (IsTrailingHit)
			{
				CaretEnd = CacheTargetPtr->size();
			}

			if (CaretEnd < CaretStart)
			{
				SelectMode = VEditorCaretSelectMode::Left;
			}

			SetValidCaretRange();

			return;
		}
		}
	}
}
void VEditorCaret::CaretSelectionTurnDown(IDWriteTextLayout *TextLayout)
{
	InSelecting = true;

	if (CaretStart == CaretEnd)
	{
		DWRITE_TEXT_METRICS				 TextMetrics;
		std::vector<DWRITE_LINE_METRICS> LineMetrics;

		TextLayout->GetMetrics(&TextMetrics);

		LineMetrics.resize(TextMetrics.lineCount);

		TextLayout->GetLineMetrics(&LineMetrics[0], TextMetrics.lineCount, &TextMetrics.lineCount);

		UINT32 Line				= 0;
		UINT32 LinePosition		= 0;
		UINT32 NextLinePosition = 0;
		UINT32 LineCount		= static_cast<UINT32>(LineMetrics.size());
		for (; Line < LineCount; ++Line)
		{
			LinePosition	 = NextLinePosition;
			NextLinePosition = LinePosition + LineMetrics[Line].length;

			if (NextLinePosition > CaretStart)
			{
				break;
			}
		}

		if (Line > LineCount - 1)
		{
			Line = LineCount - 1;
		}

		LinePosition += LineMetrics[Line].length;

		++Line;

		if (Line >= LineMetrics.size())
		{
			return;
		}

		DWRITE_HIT_TEST_METRICS HitTestMetrics;

		FLOAT CaretX		  = 0;
		FLOAT CaretY		  = 0;
		FLOAT UnusedParemeter = 0;

		BOOL IsInside	   = FALSE;
		BOOL IsTrailingHit = FALSE;

		TextLayout->HitTestTextPosition(CaretStart, FALSE, &CaretX, &CaretY, &HitTestMetrics);
		TextLayout->HitTestTextPosition(LinePosition, FALSE, &UnusedParemeter, &CaretY, &HitTestMetrics);
		TextLayout->HitTestPoint(CaretX, CaretY, &IsTrailingHit, &IsInside, &HitTestMetrics);

		CaretEnd = HitTestMetrics.textPosition;

		if (IsTrailingHit)
		{
			CaretEnd = CacheTargetPtr->size();
		}

		SelectMode = VEditorCaretSelectMode::Right;

		return;
	}
	else
	{
		switch (SelectMode)
		{
		case VEditorCaretSelectMode::Left: {
			DWRITE_TEXT_METRICS				 TextMetrics;
			std::vector<DWRITE_LINE_METRICS> LineMetrics;

			TextLayout->GetMetrics(&TextMetrics);

			LineMetrics.resize(TextMetrics.lineCount);

			TextLayout->GetLineMetrics(&LineMetrics[0], TextMetrics.lineCount, &TextMetrics.lineCount);

			UINT32 Line				= 0;
			UINT32 LinePosition		= 0;
			UINT32 NextLinePosition = 0;
			UINT32 LineCount		= static_cast<UINT32>(LineMetrics.size());
			for (; Line < LineCount; ++Line)
			{
				LinePosition	 = NextLinePosition;
				NextLinePosition = LinePosition + LineMetrics[Line].length;

				if (NextLinePosition > CaretStart)
				{
					break;
				}
			}

			if (Line > LineCount - 1)
			{
				Line = LineCount - 1;
			}

			LinePosition += LineMetrics[Line].length;

			++Line;

			if (Line >= LineMetrics.size())
			{
				return;
			}

			DWRITE_HIT_TEST_METRICS HitTestMetrics;

			FLOAT CaretX		  = 0;
			FLOAT CaretY		  = 0;
			FLOAT UnusedParemeter = 0;

			BOOL IsInside	   = FALSE;
			BOOL IsTrailingHit = FALSE;

			TextLayout->HitTestTextPosition(CaretStart, FALSE, &CaretX, &CaretY, &HitTestMetrics);
			TextLayout->HitTestTextPosition(LinePosition, FALSE, &UnusedParemeter, &CaretY, &HitTestMetrics);
			TextLayout->HitTestPoint(CaretX, CaretY, &IsTrailingHit, &IsInside, &HitTestMetrics);

			CaretStart = HitTestMetrics.textPosition;

			if (CaretStart > CaretEnd)
			{
				SelectMode = VEditorCaretSelectMode::Right;
			}

			SetValidCaretRange();

			return;
		}
		case VEditorCaretSelectMode::Right: {
			DWRITE_TEXT_METRICS				 TextMetrics;
			std::vector<DWRITE_LINE_METRICS> LineMetrics;

			TextLayout->GetMetrics(&TextMetrics);

			LineMetrics.resize(TextMetrics.lineCount);

			TextLayout->GetLineMetrics(&LineMetrics[0], TextMetrics.lineCount, &TextMetrics.lineCount);

			UINT32 Line				= 0;
			UINT32 LinePosition		= 0;
			UINT32 NextLinePosition = 0;
			UINT32 LineCount		= static_cast<UINT32>(LineMetrics.size());
			for (; Line < LineCount; ++Line)
			{
				LinePosition	 = NextLinePosition;
				NextLinePosition = LinePosition + LineMetrics[Line].length;

				if (NextLinePosition > CaretEnd)
				{
					break;
				}
			}

			if (Line > LineCount - 1)
			{
				Line = LineCount - 1;
			}

			LinePosition += LineMetrics[Line].length;

			++Line;

			if (Line >= LineMetrics.size())
			{
				return;
			}

			DWRITE_HIT_TEST_METRICS HitTestMetrics;

			FLOAT CaretX		  = 0;
			FLOAT CaretY		  = 0;
			FLOAT UnusedParemeter = 0;

			BOOL IsInside	   = FALSE;
			BOOL IsTrailingHit = FALSE;

			TextLayout->HitTestTextPosition(CaretEnd, FALSE, &CaretX, &CaretY, &HitTestMetrics);
			TextLayout->HitTestTextPosition(LinePosition, FALSE, &UnusedParemeter, &CaretY, &HitTestMetrics);
			TextLayout->HitTestPoint(CaretX, CaretY, &IsTrailingHit, &IsInside, &HitTestMetrics);

			CaretEnd = HitTestMetrics.textPosition;

			SetValidCaretRange();

			return;
		}
		}
	}
}
void VEditorCaret::CaretSelectionTurnLeft()
{
	InSelecting = true;

	if (CaretStart == CaretEnd)
	{
		SelectMode = VEditorCaretSelectMode::Left;

		if (CaretStart - 1 >= 0)
		{
			CaretStart -= 1;
		}

		return;
	}
	else
	{
		switch (SelectMode)
		{
		case VEditorCaretSelectMode::Left: {
			if (CaretStart - 1 >= 0)
			{
				CaretStart -= 1;
			}

			return;
		}
		case VEditorCaretSelectMode::Right: {
			if (CaretEnd - 1 >= 0)
			{
				CaretEnd -= 1;

				if (CaretEnd <= CaretStart)
				{
					SelectMode = VEditorCaretSelectMode::Right;
				}
			}

			return;
		}
		}
	}

	SetValidCaretRange();
}
void VEditorCaret::CaretSelectionTurnRight()
{
	InSelecting = true;

	if (CaretStart == CaretEnd)
	{
		SelectMode = VEditorCaretSelectMode::Right;

		if (CaretEnd + 1 <= CacheTargetPtr->size())
		{
			CaretEnd += 1;
		}

		return;
	}
	else
	{
		switch (SelectMode)
		{
		case VEditorCaretSelectMode::Left: {
			if (CaretStart + 1 <= CacheTargetPtr->size())
			{
				CaretStart += 1;

				if (CaretStart >= CaretEnd)
				{
					SelectMode = VEditorCaretSelectMode::Right;
				}
			}

			return;
		}
		case VEditorCaretSelectMode::Right: {
			if (CaretEnd + 1 <= CacheTargetPtr->size())
			{
				CaretEnd += 1;
			}

			return;
		}
		}
	}

	SetValidCaretRange();
}

void VEditorCaret::ResetSelect()
{
	CaretStart = 0;
	CaretEnd   = CaretStart;
}

void VEditorCaret::CaretTurnUp(IDWriteTextLayout *TextLayout)
{
	if (CaretStart == CaretEnd)
	{
		DWRITE_TEXT_METRICS				 TextMetrics;
		std::vector<DWRITE_LINE_METRICS> LineMetrics;

		TextLayout->GetMetrics(&TextMetrics);

		LineMetrics.resize(TextMetrics.lineCount);

		TextLayout->GetLineMetrics(&LineMetrics[0], TextMetrics.lineCount, &TextMetrics.lineCount);

		UINT32 Line				= 0;
		UINT32 LinePosition		= 0;
		UINT32 NextLinePosition = 0;
		UINT32 LineCount		= static_cast<UINT32>(LineMetrics.size());
		for (; Line < LineCount; ++Line)
		{
			LinePosition	 = NextLinePosition;
			NextLinePosition = LinePosition + LineMetrics[Line].length;

			if (NextLinePosition > CaretStart)
			{
				break;
			}
		}

		if (Line > LineCount - 1)
		{
			Line = LineCount - 1;
		}

		if (Line <= 0)
		{
			return;
		}

		--Line;
		LinePosition -= LineMetrics[Line].length;

		DWRITE_HIT_TEST_METRICS HitTestMetrics;

		FLOAT CaretX		  = 0;
		FLOAT CaretY		  = 0;
		FLOAT UnusedParemeter = 0;

		BOOL IsInside	   = FALSE;
		BOOL IsTrailingHit = FALSE;

		TextLayout->HitTestTextPosition(CaretStart, FALSE, &CaretX, &CaretY, &HitTestMetrics);
		TextLayout->HitTestTextPosition(LinePosition, FALSE, &UnusedParemeter, &CaretY, &HitTestMetrics);

		CaretY += 12;

		TextLayout->HitTestPoint(CaretX, CaretY, &IsTrailingHit, &IsInside, &HitTestMetrics);

		CaretStart = HitTestMetrics.textPosition;

		CaretEnd = CaretStart;

		return;
	}
	else
	{
		switch (SelectMode)
		{
		case VEditorCaretSelectMode::Left: {
			DWRITE_TEXT_METRICS				 TextMetrics;
			std::vector<DWRITE_LINE_METRICS> LineMetrics;

			TextLayout->GetMetrics(&TextMetrics);

			LineMetrics.resize(TextMetrics.lineCount);

			TextLayout->GetLineMetrics(&LineMetrics[0], TextMetrics.lineCount, &TextMetrics.lineCount);

			UINT32 Line				= 0;
			UINT32 LinePosition		= 0;
			UINT32 NextLinePosition = 0;
			UINT32 LineCount		= static_cast<UINT32>(LineMetrics.size());
			for (; Line < LineCount; ++Line)
			{
				LinePosition	 = NextLinePosition;
				NextLinePosition = LinePosition + LineMetrics[Line].length;

				if (NextLinePosition > CaretStart)
				{
					break;
				}
			}

			if (Line > LineCount - 1)
			{
				Line = LineCount - 1;
			}

			if (Line <= 0)
			{
				return;
			}

			--Line;
			LinePosition -= LineMetrics[Line].length;

			DWRITE_HIT_TEST_METRICS HitTestMetrics;

			FLOAT CaretX		  = 0;
			FLOAT CaretY		  = 0;
			FLOAT UnusedParemeter = 0;

			BOOL IsInside	   = FALSE;
			BOOL IsTrailingHit = FALSE;

			TextLayout->HitTestTextPosition(CaretStart, FALSE, &CaretX, &CaretY, &HitTestMetrics);
			TextLayout->HitTestTextPosition(LinePosition, FALSE, &UnusedParemeter, &CaretY, &HitTestMetrics);

			CaretY += 12;

			TextLayout->HitTestPoint(CaretX, CaretY, &IsTrailingHit, &IsInside, &HitTestMetrics);

			CaretStart = HitTestMetrics.textPosition;

			CaretEnd = CaretStart;

			return;
		}
		case VEditorCaretSelectMode::Right: {
			DWRITE_TEXT_METRICS				 TextMetrics;
			std::vector<DWRITE_LINE_METRICS> LineMetrics;

			TextLayout->GetMetrics(&TextMetrics);

			LineMetrics.resize(TextMetrics.lineCount);

			TextLayout->GetLineMetrics(&LineMetrics[0], TextMetrics.lineCount, &TextMetrics.lineCount);

			UINT32 Line				= 0;
			UINT32 LinePosition		= 0;
			UINT32 NextLinePosition = 0;
			UINT32 LineCount		= static_cast<UINT32>(LineMetrics.size());
			for (; Line < LineCount; ++Line)
			{
				LinePosition	 = NextLinePosition;
				NextLinePosition = LinePosition + LineMetrics[Line].length;

				if (NextLinePosition > CaretEnd)
				{
					break;
				}
			}

			if (Line > LineCount - 1)
			{
				Line = LineCount - 1;
			}

			if (Line <= 0)
			{
				return;
			}

			--Line;
			LinePosition -= LineMetrics[Line].length;

			DWRITE_HIT_TEST_METRICS HitTestMetrics;

			FLOAT CaretX		  = 0;
			FLOAT CaretY		  = 0;
			FLOAT UnusedParemeter = 0;

			BOOL IsInside	   = FALSE;
			BOOL IsTrailingHit = FALSE;

			TextLayout->HitTestTextPosition(CaretEnd, FALSE, &CaretX, &CaretY, &HitTestMetrics);
			TextLayout->HitTestTextPosition(LinePosition, FALSE, &UnusedParemeter, &CaretY, &HitTestMetrics);

			CaretY += 12;

			TextLayout->HitTestPoint(CaretX, CaretY, &IsTrailingHit, &IsInside, &HitTestMetrics);

			CaretEnd = HitTestMetrics.textPosition;

			CaretStart = CaretEnd;

			return;
		}
		}
	}
}
void VEditorCaret::CaretTurnDown(IDWriteTextLayout *TextLayout)
{
	if (CaretStart == CaretEnd)
	{
		DWRITE_TEXT_METRICS				 TextMetrics;
		std::vector<DWRITE_LINE_METRICS> LineMetrics;

		TextLayout->GetMetrics(&TextMetrics);

		LineMetrics.resize(TextMetrics.lineCount);

		TextLayout->GetLineMetrics(&LineMetrics[0], TextMetrics.lineCount, &TextMetrics.lineCount);

		UINT32 Line				= 0;
		UINT32 LinePosition		= 0;
		UINT32 NextLinePosition = 0;
		UINT32 LineCount		= static_cast<UINT32>(LineMetrics.size());
		for (; Line < LineCount; ++Line)
		{
			LinePosition	 = NextLinePosition;
			NextLinePosition = LinePosition + LineMetrics[Line].length;

			if (NextLinePosition > CaretStart)
			{
				LinePosition += LineMetrics[Line].length;

				break;
			}
		}

		if (Line > LineCount - 1)
		{
			Line = LineCount - 1;
		}

		LinePosition += LineMetrics[Line].length;
		++Line;

		if (Line >= LineMetrics.size())
		{
			return;
		}

		DWRITE_HIT_TEST_METRICS HitTestMetrics;

		FLOAT CaretX		  = 0;
		FLOAT CaretY		  = 0;
		FLOAT UnusedParemeter = 0;

		BOOL IsInside	   = FALSE;
		BOOL IsTrailingHit = FALSE;

		TextLayout->HitTestTextPosition(CaretStart, FALSE, &CaretX, &CaretY, &HitTestMetrics);
		TextLayout->HitTestTextPosition(NextLinePosition, FALSE, &UnusedParemeter, &CaretY, &HitTestMetrics);

		CaretY += 12;

		TextLayout->HitTestPoint(CaretX, CaretY, &IsTrailingHit, &IsInside, &HitTestMetrics);

		CaretStart = HitTestMetrics.textPosition;

		if (IsTrailingHit)
		{
			CaretStart += HitTestMetrics.length;
		}

		CaretEnd = CaretStart;

		return;
	}
	else
	{
		switch (SelectMode)
		{
		case VEditorCaretSelectMode::Left: {
			DWRITE_TEXT_METRICS				 TextMetrics;
			std::vector<DWRITE_LINE_METRICS> LineMetrics;

			TextLayout->GetMetrics(&TextMetrics);

			LineMetrics.resize(TextMetrics.lineCount);

			TextLayout->GetLineMetrics(&LineMetrics[0], TextMetrics.lineCount, &TextMetrics.lineCount);

			UINT32 Line				= 0;
			UINT32 LinePosition		= 0;
			UINT32 NextLinePosition = 0;
			UINT32 LineCount		= static_cast<UINT32>(LineMetrics.size());
			for (; Line < LineCount; ++Line)
			{
				LinePosition	 = NextLinePosition;
				NextLinePosition = LinePosition + LineMetrics[Line].length;

				if (NextLinePosition > CaretStart)
				{
					break;
				}
			}

			if (Line > LineCount - 1)
			{
				Line = LineCount - 1;
			}

			LinePosition += LineMetrics[Line].length;

			++Line;

			if (Line >= LineMetrics.size())
			{
				return;
			}

			DWRITE_HIT_TEST_METRICS HitTestMetrics;

			FLOAT CaretX		  = 0;
			FLOAT CaretY		  = 0;
			FLOAT UnusedParemeter = 0;

			BOOL IsInside	   = FALSE;
			BOOL IsTrailingHit = FALSE;

			TextLayout->HitTestTextPosition(CaretStart, FALSE, &CaretX, &CaretY, &HitTestMetrics);
			TextLayout->HitTestTextPosition(LinePosition, FALSE, &UnusedParemeter, &CaretY, &HitTestMetrics);

			CaretY += 12;

			TextLayout->HitTestPoint(CaretX, CaretY, &IsTrailingHit, &IsInside, &HitTestMetrics);

			CaretStart = HitTestMetrics.textPosition;

			CaretEnd = CaretStart;

			return;
		}
		case VEditorCaretSelectMode::Right: {
			DWRITE_TEXT_METRICS				 TextMetrics;
			std::vector<DWRITE_LINE_METRICS> LineMetrics;

			TextLayout->GetMetrics(&TextMetrics);

			LineMetrics.resize(TextMetrics.lineCount);

			TextLayout->GetLineMetrics(&LineMetrics[0], TextMetrics.lineCount, &TextMetrics.lineCount);

			UINT32 Line				= 0;
			UINT32 LinePosition		= 0;
			UINT32 NextLinePosition = 0;
			UINT32 LineCount		= static_cast<UINT32>(LineMetrics.size());
			for (; Line < LineCount; ++Line)
			{
				LinePosition	 = NextLinePosition;
				NextLinePosition = LinePosition + LineMetrics[Line].length;

				if (NextLinePosition > CaretEnd)
				{
					break;
				}
			}

			if (Line > LineCount - 1)
			{
				Line = LineCount - 1;
			}

			LinePosition += LineMetrics[Line].length;

			++Line;

			if (Line >= LineMetrics.size())
			{
				return;
			}

			DWRITE_HIT_TEST_METRICS HitTestMetrics;

			FLOAT CaretX		  = 0;
			FLOAT CaretY		  = 0;
			FLOAT UnusedParemeter = 0;

			BOOL IsInside	   = FALSE;
			BOOL IsTrailingHit = FALSE;

			TextLayout->HitTestTextPosition(CaretEnd, FALSE, &CaretX, &CaretY, &HitTestMetrics);
			TextLayout->HitTestTextPosition(LinePosition, FALSE, &UnusedParemeter, &CaretY, &HitTestMetrics);

			CaretY += 12;

			TextLayout->HitTestPoint(CaretX, CaretY, &IsTrailingHit, &IsInside, &HitTestMetrics);

			CaretStart = HitTestMetrics.textPosition;

			CaretEnd = CaretStart;

			return;
		}
		}
	}
}
void VEditorCaret::CaretTurnLineHead(IDWriteTextLayout *TextLayout)
{
	if (CaretStart != CaretEnd)
	{
		CaretStart = max(CaretStart, CaretEnd);
		CaretEnd   = CaretStart;
	}

	DWRITE_TEXT_METRICS				 TextMetrics;
	std::vector<DWRITE_LINE_METRICS> LineMetrics;

	TextLayout->GetMetrics(&TextMetrics);

	LineMetrics.resize(TextMetrics.lineCount);

	TextLayout->GetLineMetrics(&LineMetrics[0], TextMetrics.lineCount, &TextMetrics.lineCount);

	UINT32 Line			= 0;
	UINT32 LinePosition = 0;
	UINT32 LineCount	= static_cast<UINT32>(LineMetrics.size());
	for (; Line < LineCount; ++Line)
	{
		LinePosition += LineMetrics[Line].length;

		if (LinePosition > CaretStart)
		{
			break;
		}
	}

	if (Line >= LineCount)
	{
		Line = LineCount - 1;
	}

	LinePosition -= LineMetrics[Line].length;

	CaretStart = LinePosition;
	CaretEnd   = CaretStart;
}
void VEditorCaret::CaretTurnLineEnd(IDWriteTextLayout *TextLayout)
{
	if (CaretStart != CaretEnd)
	{
		CaretStart = max(CaretStart, CaretEnd);
		CaretEnd   = CaretStart;
	}

	DWRITE_TEXT_METRICS				 TextMetrics;
	std::vector<DWRITE_LINE_METRICS> LineMetrics;

	TextLayout->GetMetrics(&TextMetrics);

	LineMetrics.resize(TextMetrics.lineCount);

	TextLayout->GetLineMetrics(&LineMetrics[0], TextMetrics.lineCount, &TextMetrics.lineCount);

	UINT32 Line			= 0;
	UINT32 LinePosition = 0;
	UINT32 LineCount	= static_cast<UINT32>(LineMetrics.size());
	for (; Line < LineCount; ++Line)
	{
		LinePosition += LineMetrics[Line].length;

		if (LinePosition > CaretStart)
		{
			break;
		}
	}

	CaretStart = LinePosition - 1;

	if (LinePosition >= CacheTargetPtr->size())
	{
		++CaretStart;
	}

	CaretEnd = CaretStart;
}
void VEditorCaret::CaretSelectionTurnLineHead(IDWriteTextLayout *TextLayout)
{
	DWRITE_TEXT_METRICS				 TextMetrics;
	std::vector<DWRITE_LINE_METRICS> LineMetrics;

	TextLayout->GetMetrics(&TextMetrics);

	LineMetrics.resize(TextMetrics.lineCount);

	TextLayout->GetLineMetrics(&LineMetrics[0], TextMetrics.lineCount, &TextMetrics.lineCount);

	int CaretPosition = CaretStart;

	if (SelectMode == VEditorCaretSelectMode::Right)
	{
		CaretPosition = CaretEnd;
	}

	UINT32 Line			= 0;
	UINT32 LinePosition = 0;
	UINT32 LineCount	= static_cast<UINT32>(LineMetrics.size());
	for (; Line < LineCount; ++Line)
	{
		LinePosition += LineMetrics[Line].length;

		if (LinePosition > CaretPosition)
		{
			break;
		}
	}

	if (Line >= LineMetrics.size())
	{
		Line = LineMetrics.size() - 1;
	}

	LinePosition -= LineMetrics[Line].length;

	if (SelectMode == VEditorCaretSelectMode::Right)
	{
		CaretEnd = LinePosition;
	}
	else
	{
		CaretStart = LinePosition;
	}

	InSelecting = true;

	if (CaretStart > CaretEnd)
	{
		SelectMode = VEditorCaretSelectMode::Left;
	}

	SetValidCaretRange();
}
void VEditorCaret::CaretSelectionTurnLineEnd(IDWriteTextLayout *TextLayout)
{
	DWRITE_TEXT_METRICS				 TextMetrics;
	std::vector<DWRITE_LINE_METRICS> LineMetrics;

	TextLayout->GetMetrics(&TextMetrics);

	LineMetrics.resize(TextMetrics.lineCount);

	TextLayout->GetLineMetrics(&LineMetrics[0], TextMetrics.lineCount, &TextMetrics.lineCount);

	int CaretPosition = CaretStart;

	if (SelectMode == VEditorCaretSelectMode::Right)
	{
		CaretPosition = CaretEnd;
	}

	UINT32 Line			= 0;
	UINT32 LinePosition = 0;
	UINT32 LineCount	= static_cast<UINT32>(LineMetrics.size());
	for (; Line < LineCount; ++Line)
	{
		LinePosition += LineMetrics[Line].length;

		if (LinePosition > CaretPosition)
		{
			break;
		}
	}

	if (SelectMode == VEditorCaretSelectMode::Right)
	{
		CaretEnd = LinePosition - 1;
	}
	else
	{
		CaretStart = LinePosition - 1;
	}

	InSelecting = true;

	if (CaretStart > CaretEnd)
	{
		SelectMode = VEditorCaretSelectMode::Right;
	}

	SetValidCaretRange();
}
void VEditorCaret::CaretSelectAll()
{
	CaretStart = 0;
	CaretEnd   = CacheTargetPtr->size();

	InSelecting = true;
	SelectMode	= VEditorCaretSelectMode::Right;
}
void VEditorCaret::CaretPageUp()
{
	CaretStart = 0;
	CaretEnd   = CaretStart;
}
void VEditorCaret::CaretPageEnd()
{
	CaretStart = CacheTargetPtr->size();
	CaretEnd   = CaretStart;
}
void VEditorCaret::CaretTurnLeft()
{
	if (CaretStart != CaretEnd)
	{
		CaretEnd = CaretStart;
	}
	else if (CaretStart - 1 >= 0)
	{
		CaretStart -= 1;
		CaretEnd = CaretStart;
	}
}
void VEditorCaret::CaretTurnRight()
{
	if (CaretStart != CaretEnd)
	{
		CaretStart = CaretEnd;
	}
	else if (CaretEnd + 1 <= CacheTargetPtr->size())
	{
		CaretEnd += 1;
		CaretStart = CaretEnd;
	}
}

void VEditorCaret::SetCaretByMousePosition(const int &X, const int &Y, IDWriteTextLayout *TextLayout)
{
	BOOL					IsTrailingHit = FALSE;
	BOOL					IsInside	  = FALSE;
	DWRITE_HIT_TEST_METRICS HitTestMetrics;

	TextLayout->HitTestPoint(X, Y, &IsTrailingHit, &IsInside, &HitTestMetrics);

	if (IsTrailingHit)
	{
		CaretStart = HitTestMetrics.textPosition + HitTestMetrics.length;
	}
	else
	{
		CaretStart = HitTestMetrics.textPosition;
	}

	CaretEnd = CaretStart;
}
void VEditorCaret::SetCaretSelectionByMousePosition(const int &X, const int &Y, IDWriteTextLayout *TextLayout)
{
	BOOL					IsTrailingHit = FALSE;
	BOOL					IsInside	  = FALSE;
	DWRITE_HIT_TEST_METRICS HitTestMetrics;

	TextLayout->HitTestPoint(X, Y, &IsTrailingHit, &IsInside, &HitTestMetrics);
	if (IsTrailingHit)
	{
		HitTestMetrics.textPosition += HitTestMetrics.length;
	}

	if (!InSelecting)
	{
		if (HitTestMetrics.textPosition > CaretEnd)
		{
			CaretStart = CaretEnd;

			InSelecting = true;

			SelectMode = VEditorCaretSelectMode::Right;

			CaretEnd = HitTestMetrics.textPosition;
		}
		else if (HitTestMetrics.textPosition < CaretStart)
		{
			InSelecting = true;

			SelectMode = VEditorCaretSelectMode::Left;

			CaretStart = HitTestMetrics.textPosition;
		}
		else
		{
			CaretEnd = HitTestMetrics.textPosition;
		}
	}
	else
	{
		if (HitTestMetrics.textPosition > CaretEnd)
		{
			if (SelectMode == VEditorCaretSelectMode::Left)
			{
				CaretStart = CaretEnd;

				SelectMode = VEditorCaretSelectMode::Right;

				CaretEnd = HitTestMetrics.textPosition;
			}
			else
			{
				CaretEnd = HitTestMetrics.textPosition;
			}
		}
		else if (HitTestMetrics.textPosition < CaretStart)
		{
			if (SelectMode == VEditorCaretSelectMode::Right)
			{
				CaretEnd = CaretStart;

				SelectMode = VEditorCaretSelectMode::Left;

				CaretStart = HitTestMetrics.textPosition;
			}
			else
			{
				CaretStart = HitTestMetrics.textPosition;
			}
		}
		else
		{
			if (SelectMode == VEditorCaretSelectMode::Right)
			{
				CaretEnd = HitTestMetrics.textPosition;
			}
			else
			{
				CaretStart = HitTestMetrics.textPosition;
			}
		}
	}
}

void VEditorCaret::SetValidCaretRange()
{
	if (CaretEnd < CaretStart)
	{
		std::swap(CaretEnd, CaretStart);
	}
}
VPoint VEditorCaret::GetCaretPosition(IDWriteTextLayout *TextLayout)
{
	DWRITE_HIT_TEST_METRICS CaretMetrics;

	FLOAT CaretX = 0.f;
	FLOAT CaretY = 0.f;

	if (!InSelecting)
	{
		TextLayout->HitTestTextPosition(CaretStart, FALSE, &CaretX, &CaretY, &CaretMetrics);
	}
	else
	{
		switch (SelectMode)
		{
		case VEditorCaretSelectMode::Left: {
			TextLayout->HitTestTextPosition(CaretStart, FALSE, &CaretX, &CaretY, &CaretMetrics);

			break;
		}
		case VEditorCaretSelectMode::Right: {
			TextLayout->HitTestTextPosition(CaretEnd, FALSE, &CaretX, &CaretY, &CaretMetrics);

			break;
		}
		}
	}

	return VPoint(CaretMetrics.left, CaretMetrics.top);
}
DWRITE_TEXT_RANGE VEditorCaret::GetTextRange()
{
	if (CaretEnd < CaretStart)
	{
		std::swap(CaretEnd, CaretStart);
	}

	UINT32 End	 = CaretEnd;
	UINT32 Start = CaretStart;

	return DWRITE_TEXT_RANGE{Start, End - Start};
}

VEditor::VEditor(VUIObject *Parent) : VAbstractButton(Parent), CaretTimer(this), ShowCaret(false)
{
	InitEditor();
}
VEditor::VEditor(const int &Width, const int &Height, VUIObject *Parent)
	: VAbstractButton(Parent), CaretTimer(this), ShowCaret(false)
{
	VUIObject::Resize(Width, Height);

	InitEditor();
}
void VEditor::InitEditor()
{
	Theme		 = new VTextEditorTheme(*(static_cast<VTextEditorTheme *>(GetTargetTheme(VUIThemeType::VTextEditor))));
	Interpolator = new VAnimationInterpolator(0.1, Theme->LocalTheme.AnimationInterpolatorType);

	Theme->LabelFont->SetParagraphAlignment(VFontParagraphAlignment::DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	FirstKeyPress = true;

	UsedComboKey = false;

	OffsetX = 5;
	YDelta	= 25;

	UserInOperating = false;

	Caret.CacheTargetPtr = &InEditingText;
	Caret.InSelecting	 = false;
	InMouseDragSelecting = false;

	DragResetFontSize = false;

	ResetTextLayout();

	AllowEdit = true;

	EnableOperationBack = true;
	OperationCacheMax	= 20;

	CaretTimer.Start(1000);
	CaretTimer.OnTime.Connect(this, &VEditor::ResetCaretStatus);

	TextBeforeChange.Connect(this, &VEditor::CacheOperation);
}

void VEditor::CacheOperation(const VString &OldText)
{
	if (OperationCacheMax > 0 && OperationCacheMax < OldStringSet.size() + 1)
	{
		OldStringSet.erase(OldStringSet.begin());
		CaretSet.erase(CaretSet.begin());
	}

	OldStringSet.push_back(OldText);
	CaretSet.push_back(Caret);
}

void VEditor::ResetCaretStatus()
{
	if (UserInOperating)
	{
		ShowCaret = !ShowCaret;
	}
	else
	{
		ShowCaret = false;
	}

	CaretTimer.Start(500);

	Update();
}
void VEditor::ResetTextLayout()
{
	LocalTextLayout.Reset();

	if (GetWidth() > 0 && GetHeight() > 0)
	{
		auto Result = VDirectXWriteFactory.GetInstance()->CreateTextLayout(
			InEditingText.CStyleString(), InEditingText.size(), Theme->LabelFont->GetDXObject(),
			GetWidth() - Theme->LocalTheme.BorderThickness * 2, GetHeight() - Theme->LocalTheme.BorderThickness * 2,
			LocalTextLayout.GetAddressOf());

		for (auto &Effect : TextEffect)
		{
			LocalTextLayout->SetDrawingEffect(std::get<0>(Effect), std::get<1>(Effect));
		}
		for (auto &Effect : TextStyle)
		{
			LocalTextLayout->SetFontStyle(std::get<0>(Effect), std::get<1>(Effect));
		}
		for (auto &Effect : UnderlineStyle)
		{
			LocalTextLayout->SetUnderline(true, Effect);
		}

		Update();
	}
}

bool VEditor::GetAllowEditStatus() const
{
	return AllowEdit;
}
void VEditor::SetAllowEditStatus(const bool &Status)
{
	AllowEdit = Status;
}

bool VEditor::GetAllowFontSizeDragStatus() const
{
	return DragResetFontSize;
}
void VEditor::SetAllowFontSizeDragStatus(const bool &Status)
{
	DragResetFontSize = Status;
}

bool VEditor::GetOperationBackStatus() const
{
	return EnableOperationBack;
}
void VEditor::SetOperationBackStatus(const bool &Status)
{
	EnableOperationBack = Status;
}

int VEditor::GetMaxOperationCache() const
{
	return OperationCacheMax;
}
void VEditor::SetMaxOperationCache(const int &Status)
{
	OperationCacheMax = Status;
}

void VEditor::ScrollToEnd()
{
	OffsetY = GetMaxOffsetY();
}
void VEditor::SetPlainText(const VString &PlainText)
{
	InEditingText	 = PlainText;
	Caret.CaretStart = 0;
	Caret.CaretEnd	 = 0;

	ResetTextLayout();

	TextOnChange.Emit(GetPlainText());
}
void VEditor::SetLeadingText(const VString &PlainText)
{
	LeadText		 = PlainText;
	Caret.CaretStart = 0;
	Caret.CaretEnd	 = 0;

	ResetTextLayout();

	TextOnChange.Emit(GetPlainText());
}
void VEditor::DrawSelectedRange(Core::VCanvasPainter *Painter)
{
	auto HitCount	 = UINT32(0);
	auto SelectRange = Caret.GetTextRange();

	if (SelectRange.length > 0 && Caret.InSelecting)
	{
		LocalTextLayout->HitTestTextRange(SelectRange.startPosition, SelectRange.length, 0, 0, 0, 0, &HitCount);

		auto HitTestMetrics = std::vector<DWRITE_HIT_TEST_METRICS>(HitCount);

		LocalTextLayout->HitTestTextRange(SelectRange.startPosition, SelectRange.length, 0, OffsetY, &HitTestMetrics[0],
										  static_cast<UINT32>(HitTestMetrics.size()), &HitCount);

		VSolidBrush FillRectangleBrush(Theme->OnSelectedBackgroundColor, CallWidgetGetRenderHandle());
		for (auto Count = UINT32(0); Count < HitCount; ++Count)
		{
			auto HitTestMetric = HitTestMetrics[Count];

			HitTestMetric.left += OffsetX;
			HitTestMetric.top += OffsetY + Theme->LabelFont->GetTextSize();

			if (HitTestMetric.width == 0)
			{
				HitTestMetric.width = 10;
			}

			Painter->SolidRectangle(VRect(HitTestMetric.left, HitTestMetric.top,
										  HitTestMetric.left + HitTestMetric.width,
										  HitTestMetric.top + HitTestMetric.height),
									&FillRectangleBrush);
		}
	}
}
void VEditor::OnPaint(VCanvasPainter *Painter)
{
	if (GetWidth() == 0 || GetHeight() == 0)
	{
		return;
	}

	Painter->BeginDraw();

	VSolidBrush TextBrush(Theme->LocalTheme.TextColor, CallWidgetGetRenderHandle());
	VSolidBrush BackgroundBrush(Theme->LocalTheme.BackgroundColor, CallWidgetGetRenderHandle());
	VPenBrush	SelectedBrush(Theme->OnSelectedColor, CallWidgetGetRenderHandle());
	VPenBrush	CaretBrush(Theme->CursorColor, CallWidgetGetRenderHandle(), 1.f);
	VPenBrush	BorderBrush(Theme->LocalTheme.BorderColor, CallWidgetGetRenderHandle(),
							Theme->LocalTheme.BorderThickness);

	if (Theme->OnSelectedColor.GetA() != 0)
	{
		LocalTextLayout->SetDrawingEffect(SelectedBrush.GetDxBrush(), Caret.GetTextRange());
	}

	Painter->FillRoundedRectangle(VRect(Theme->LocalTheme.BorderThickness, Theme->LocalTheme.BorderThickness,
										GetWidth() - Theme->LocalTheme.BorderThickness,
										GetHeight() - Theme->LocalTheme.BorderThickness),
								  Theme->LocalTheme.Radius, &BorderBrush, &BackgroundBrush);

	if (!InEditingText.empty())
	{
		DrawSelectedRange(Painter);

		Painter->GetDXObject()->DrawTextLayout(D2D1::Point2F(OffsetX, OffsetY + Theme->LabelFont->GetTextSize()),
											   LocalTextLayout.Get(), TextBrush.GetDxBrush());
	}
	else
	{
		VSolidBrush LeadingTextBrush(Theme->PlaceHolderColor, CallWidgetGetRenderHandle());
		Microsoft::WRL::ComPtr<IDWriteTextLayout> LeadingTextLayout;

		VDirectXWriteFactory.GetInstance()->CreateTextLayout(
			LeadText.CStyleString(), LeadText.size(), Theme->LabelFont->GetDXObject(),
			GetWidth() - Theme->LocalTheme.BorderThickness * 2, GetHeight() - Theme->LocalTheme.BorderThickness * 2,
			LeadingTextLayout.GetAddressOf());
		Painter->GetDXObject()->DrawTextLayout(D2D1::Point2F(OffsetX, OffsetY + Theme->LabelFont->GetTextSize()),
											   LeadingTextLayout.Get(), LeadingTextBrush.GetDxBrush());
	}

	if (ShowCaret)
	{
		auto CaretPosition = Caret.GetCaretPosition(LocalTextLayout.Get());

		CaretPosition.Offset(OffsetX, OffsetY + Theme->LabelFont->GetTextSize() + 4);

		auto CaretEndPosition = CaretPosition;
		CaretEndPosition.Offset(0, Theme->LabelFont->GetTextSize() - 2);

		Painter->DrawLine(CaretPosition, CaretEndPosition, &CaretBrush);
	}

	Painter->EndDraw();

	ResetTextLayout();
}

void VEditor::LeftClickedDown()
{
	InAnimation		= true;
	UserInOperating = true;
	ShowCaret		= true;

	OldTheme	= Theme->LocalTheme;
	TargetTheme = Theme->ActiveTheme;

	Interpolator->Reset();
	AnimationFrameTimer.Start(0);

	CallWidgetSetFocusID(ObjectKernel.GlobalID);

	Update();
}
void VEditor::GotMouseFocus()
{
	InAnimation = true;

	OldTheme	= Theme->LocalTheme;
	TargetTheme = Theme->OnHoverTheme;

	Interpolator->Reset();
	AnimationFrameTimer.Start(0);

	CallWidgetSetFocusID(ObjectKernel.GlobalID);

	Update();
}

void VEditor::SetDeltaY(const int &Delta)
{
	YDelta = Delta;
}
void VEditor::SetLeftMargin(const int &LeftMargin)
{
	OffsetX = LeftMargin;
}

void VEditor::CheckFrame()
{
	if (KeyPressResetTimer.End() && time(NULL) - LastKeyPressTime >= 1000)
	{
		FirstKeyPress = true;
	}
	if (UserInOperating)
	{
		auto CaretPosition = Caret.GetCaretPosition(LocalTextLayout.Get());
		CallWidgetSetIME(GetOriginX() + CaretPosition.X + 5,
						 GetOriginY() + CaretPosition.Y + Theme->LabelFont->GetTextSize());
	}
	if (!Interpolator->IsEnd() && InAnimation)
	{
		if (AnimationFrameTimer.End())
		{
			AnimationFrameTimer.Start(16);

			auto AnimationCurvature = Interpolator->GetOneFrame();

			Theme->LocalTheme.BorderThickness =
				TargetTheme.BorderThickness +
				(OldTheme.BorderThickness - TargetTheme.BorderThickness) * (1.f - AnimationCurvature);

			Theme->LocalTheme.TextColor =
				VColorInterpolator::GetTheColorByPercent(OldTheme.TextColor, TargetTheme.TextColor, AnimationCurvature);
			Theme->LocalTheme.BackgroundColor = VColorInterpolator::GetTheColorByPercent(
				OldTheme.BackgroundColor, TargetTheme.BackgroundColor, AnimationCurvature);
			Theme->LocalTheme.BorderColor = VColorInterpolator::GetTheColorByPercent(
				OldTheme.BorderColor, TargetTheme.BorderColor, AnimationCurvature);

			Theme->LocalTheme.Radius.X =
				TargetTheme.Radius.X + (OldTheme.Radius.X - TargetTheme.Radius.X) * (1.f - AnimationCurvature);
			Theme->LocalTheme.Radius.Y =
				TargetTheme.Radius.Y + (OldTheme.Radius.Y - TargetTheme.Radius.Y) * (1.f - AnimationCurvature);

			Update();
		}
	}
	else if (InAnimation)
	{
		InAnimation = false;
	}
}

void VEditor::BackCharacter()
{
	if (AllowEdit)
	{
		if (ClearSelectArea())
		{
			return;
		}

		if (Caret.CaretStart - 1 >= 0)
		{
			InEditingText.Erase(InEditingText.begin() + Caret.CaretStart - 1);
		}

		Caret.CaretTurnLeft();

		ResetTextLayout();
	}
}
void VEditor::DeleteCharacter()
{
	if (AllowEdit)
	{
		if (ClearSelectArea())
		{
			return;
		}

		if (Caret.CaretStart < InEditingText.Size())
		{
			InEditingText.Erase(InEditingText.Begin() + Caret.CaretStart);
		}

		ResetTextLayout();
	}
}
void VEditor::AddCharaceter(const wchar_t &Character)
{
	if (AllowEdit)
	{
		ClearSelectArea();

		if (Character != L'\r')
		{
			InEditingText.Insert(InEditingText.Begin() + Caret.CaretStart, Character);

			Caret.CaretTurnRight();

			ResetTextLayout();
		}
		else
		{
			InEditingText.Insert(InEditingText.begin() + Caret.CaretStart, L'\n');

			Caret.CaretTurnRight();

			ResetTextLayout();
		}
	}
}
void VEditor::ResetOffsetYByCaret()
{
	auto CaretY = Caret.GetCaretPosition(LocalTextLayout.Get()).Y;

	if (CaretY >= GetHeight())
	{
		OffsetY = CaretY;
	}
	else
	{
		OffsetY = 0;
	}
}
bool VEditor::ClearSelectArea()
{
	if (Caret.InSelecting && AllowEdit)
	{
		Caret.InSelecting = false;

		InEditingText.Erase(Caret.CaretStart, Caret.CaretEnd - Caret.CaretStart);

		Caret.CaretEnd = Caret.CaretStart;

		return true;
	}

	return false;
}

bool VEditor::IsASCIICharacter(const wchar_t &ASCIICode)
{
	return !(ASCIICode >= 0 && ASCIICode <= 31 && ASCIICode == 127);
}

void VEditor::SetScroller()
{
	if (LocalTextLayout)
	{
		auto CaretPosition = Caret.GetCaretPosition(LocalTextLayout.Get());

		if (CaretPosition.Y <= -OffsetY)
		{
			OffsetY = -CaretPosition.Y;
		}
		if (CaretPosition.Y + Theme->LabelFont->GetTextSize() * 2 + 4 >= -OffsetY + GetHeight())
		{
			OffsetY -= (CaretPosition.Y + Theme->LabelFont->GetTextSize() * 2 + 4 - (-OffsetY + GetHeight()));
		}
	}
}
int VEditor::GetMaxOffsetY()
{
	DWRITE_TEXT_METRICS Metrics;
	LocalTextLayout->GetMetrics(&Metrics);

	auto CaretCopy		 = Caret;
	CaretCopy.CaretStart = InEditingText.size() + 1;
	CaretCopy.CaretEnd	 = InEditingText.size() + 1;

	auto Result = CaretCopy.GetCaretPosition(LocalTextLayout.Get()).Y + Theme->LabelFont->GetTextSize() +
				  Theme->LabelFont->GetTextSize();

	if (Metrics.height + Theme->LabelFont->GetTextSize() > GetHeight())
	{
		return -(Result - Theme->LabelFont->GetTextSize());
	}
	else
	{
		return 0;
	}
}

void VEditor::CopyClipboard()
{
	DWRITE_TEXT_RANGE TextRange = Caret.GetTextRange();
	if (TextRange.length <= 0)
	{
		return;
	}

	if (OpenClipboard(0))
	{
		if (EmptyClipboard())
		{
			size_t	DataByteSize	= sizeof(wchar_t) * (TextRange.length + 1);
			HGLOBAL ClipboardHandle = GlobalAlloc(GMEM_DDESHARE | GMEM_ZEROINIT, DataByteSize);

			if (ClipboardHandle != NULL)
			{
				void *LockMemory = GlobalLock(ClipboardHandle);

				if (LockMemory != NULL)
				{
					const wchar_t *CString = InEditingText.CStyleString();

					memcpy(LockMemory, &CString[TextRange.startPosition], DataByteSize);

					GlobalUnlock(ClipboardHandle);
					if (SetClipboardData(CF_UNICODETEXT, ClipboardHandle) != NULL)
					{
						ClipboardHandle = NULL;
					}
				}

				GlobalFree(ClipboardHandle);
			}
		}
		CloseClipboard();
	}
}
void VEditor::WriteClipboard()
{
	if (OpenClipboard(0))
	{
		HGLOBAL ClipboardDataHandle = GetClipboardData(CF_UNICODETEXT);

		if (ClipboardDataHandle != NULL)
		{
			size_t MemoryByteSize = GlobalSize(ClipboardDataHandle);
			void  *Memory		  = GlobalLock(ClipboardDataHandle);

			const wchar_t *CString = reinterpret_cast<const wchar_t *>(Memory);

			if (CString == NULL)
			{
				return;
			}

			UINT StringCount = static_cast<UINT32>(wcsnlen(CString, MemoryByteSize / sizeof(wchar_t)));

			if (Memory != NULL)
			{
				bool AllowFlag			 = true;
				int	 ActuallyStringCount = 0;
				for (UINT Count = 0; Count < StringCount; ++Count)
				{
					CheckInput.Emit(CString[Count], &AllowFlag);

					if (AllowFlag == true)
					{
						InEditingText.Insert(InEditingText.begin() + Caret.CaretStart + ActuallyStringCount,
											 CString[Count]);

						++ActuallyStringCount;
					}
				}

				Caret.CaretStart += ActuallyStringCount;
				Caret.CaretEnd = Caret.CaretStart;

				GlobalUnlock(ClipboardDataHandle);
			}
		}

		ResetTextLayout();

		CloseClipboard();
	}
}
VTextEditorTheme *VEditor::GetTheme()
{
	return Theme;
}

VEditorCaret VEditor::GetCaret()
{
	return Caret;
}
void VEditor::SetCaret(const VEditorCaret &CaretValue)
{
	Caret = CaretValue;

	SetScroller();
	Update();
}

IDWriteTextLayout *VEditor::GetTextLayout()
{
	return LocalTextLayout.Get();
}

VString VEditor::GetPlainText() const
{
	return InEditingText;
}
VString VEditor::GetLedingText() const
{
	return LeadText;
}

void VEditor::Resize(const int &Width, const int &Height)
{
	VUIObject::Resize(Width, Height);

	ResetTextLayout();
	SetScroller();
}

void VEditor::OnMessage(VMessage *Message)
{
	if (Message->GetType() == VMessageType::KillFocusMessage && !UserInOperating)
	{
		UserInOperating		 = false;
		ShowCaret			 = false;
		InMouseDragSelecting = false;
		InAnimation			 = true;

		Caret.CaretStart = 0;
		Caret.CaretEnd	 = Caret.CaretStart;

		OldTheme	= Theme->LocalTheme;
		TargetTheme = Theme->StaticTheme;

		Caret.ResetSelect();

		Interpolator->Reset();
		AnimationFrameTimer.Start(0);

		Update();
	}
	if (Message->GetType() == VMessageType::CheckLocalFocusMessage)
	{
		auto *CheckFocus = static_cast<VCheckFocusMessage *>(Message);

		if (CheckFocus->Object != this && UserInOperating)
		{
			if (CheckFocus->Click)
			{
				UserInOperating = false;
				ShowCaret		= false;
				InAnimation		= true;

				Caret.CaretStart = 0;
				Caret.CaretEnd	 = Caret.CaretStart;

				OldTheme	= Theme->LocalTheme;
				TargetTheme = Theme->StaticTheme;

				Interpolator->Reset();
				AnimationFrameTimer.Start(0);

				Update();
			}

			HCURSOR ArrowCursor = LoadCursor(NULL, IDC_ARROW);
			SetClassLongPtr(CallWidgetGetHWND(), GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(ArrowCursor));
		}
	}
	if (Message->GetType() == VMessageType::MouseClickedMessage)
	{
		auto MouseMessage = static_cast<VMouseClickedMessage *>(Message);

		if (MouseMessage->ClickedKey == Left && MouseMessage->ClickedMethod == Down &&
			!MouseMessage->MousePosition.InsideRectangle(GetRegion()) &&
			CallWidgetGetFocusID() == ObjectKernel.GlobalID)
		{
			UserInOperating = false;
			ShowCaret		= false;
			InAnimation		= true;

			OldTheme	= Theme->LocalTheme;
			TargetTheme = Theme->StaticTheme;

			Interpolator->Reset();
			AnimationFrameTimer.Start(0);

			Update();
		}
		if (MouseMessage->ClickedKey == Left && MouseMessage->ClickedMethod == Up)
		{
			InMouseDragSelecting = false;
		}
		if (MouseMessage->ClickedKey == Left && MouseMessage->ClickedMethod == Down &&
			MouseMessage->MousePosition.InsideRectangle(GetRegion()))
		{
			auto X = MouseMessage->MousePosition.X;
			auto Y = MouseMessage->MousePosition.Y;

			X -= GetX();
			Y -= GetY();

			X -= OffsetX;

			Y -= Theme->LabelFont->GetTextSize();
			Y -= OffsetY;

			InMouseDragSelecting = true;

			if (!(GetAsyncKeyState(VK_SHIFT) & 0x8000))
			{
				Caret.SetCaretByMousePosition(X, Y, LocalTextLayout.Get());

				TextClicked.Emit(Caret.CaretStart);
			}
			else
			{
				Caret.SetCaretSelectionByMousePosition(X, Y, LocalTextLayout.Get());

				Update();

				switch (Caret.SelectMode)
				{
				case VEditorCaretSelectMode::Left: {
					TextClicked.Emit(Caret.CaretStart);

					break;
				}

				default: {
					TextClicked.Emit(Caret.CaretEnd);

					break;
				}
				}
			}
		}
	}
	if (Message->GetType() == VMessageType::MouseMoveMessage)
	{
		auto MouseMoveMessage = static_cast<VMouseMoveMessage *>(Message);

		if (MouseMoveMessage->MousePosition.InsideRectangle(GetRegion()))
		{
			HCURSOR ArrowCursor = LoadCursor(NULL, IDC_IBEAM);
			SetClassLongPtr(CallWidgetGetHWND(), GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(ArrowCursor));
		}
		else
		{
			HCURSOR ArrowCursor = LoadCursor(NULL, IDC_ARROW);
			SetClassLongPtr(CallWidgetGetHWND(), GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(ArrowCursor));
		}
	}
	if (Message->GetType() == VMessageType::MouseMoveMessage && InMouseDragSelecting)
	{
		auto MouseMoveMessage = static_cast<VMouseMoveMessage *>(Message);

		auto X = MouseMoveMessage->MousePosition.X;
		auto Y = MouseMoveMessage->MousePosition.Y;

		X -= GetX();
		Y -= GetY();

		X -= OffsetX;
		Y -= OffsetY;

		Y -= Theme->LabelFont->GetTextSize();

		Caret.SetCaretSelectionByMousePosition(X, Y, LocalTextLayout.Get());

		Update();
	}
	if (Message->GetType() == VMessageType::KeyClickedMessage && UserInOperating && Message->Win32ID == WM_KEYDOWN)
	{
		auto KeyMessage = static_cast<VKeyClickedMessage *>(Message);

		if (KeyMessage->KeyVKCode == VK_DELETE)
		{
			TextBeforeChange.Emit(InEditingText);

			DeleteCharacter();

			TextOnChange.Emit(InEditingText);
		}
		else if (KeyMessage->KeyVKCode == L'Z' && EnableOperationBack)
		{
			if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && !OldStringSet.empty())
			{
				UsedComboKey = true;

				SetPlainText(OldStringSet[OldStringSet.size() - 1]);

				Caret = CaretSet[CaretSet.size() - 1];

				OldStringSet.erase(OldStringSet.end() - 1);
				CaretSet.erase(CaretSet.end() - 1);

				TextOnChange.Emit(InEditingText);
			}
		}
		else if (KeyMessage->KeyVKCode == VK_HOME)
		{
			if (!(GetAsyncKeyState(VK_SHIFT) & 0x8000))
			{
				Caret.CaretTurnLineHead(LocalTextLayout.Get());
			}
			else
			{
				Caret.CaretSelectionTurnLineHead(LocalTextLayout.Get());
			}
		}
		else if (KeyMessage->KeyVKCode == VK_END)
		{
			if (!(GetAsyncKeyState(VK_SHIFT) & 0x8000))
			{
				Caret.CaretTurnLineEnd(LocalTextLayout.Get());
			}
			else
			{
				Caret.CaretSelectionTurnLineEnd(LocalTextLayout.Get());
			}
		}
		else if (KeyMessage->KeyVKCode == VK_PRIOR)
		{
			OffsetY = 0;
			Caret.CaretPageUp();
		}
		else if (KeyMessage->KeyVKCode == VK_NEXT)
		{
			Caret.CaretPageEnd();
		}
		else if (KeyMessage->KeyVKCode == 'A')
		{
			if ((GetAsyncKeyState(VK_CONTROL) & 0x8000))
			{
				UsedComboKey = true;

				Caret.CaretSelectAll();
				ResetOffsetYByCaret();
			}
		}
		else if (KeyMessage->KeyVKCode == 'C')
		{
			if ((GetAsyncKeyState(VK_CONTROL) & 0x8000))
			{
				UsedComboKey = true;

				CopyClipboard();
			}
		}
		else if (KeyMessage->KeyVKCode == 'X' && AllowEdit)
		{
			if ((GetAsyncKeyState(VK_CONTROL) & 0x8000))
			{
				UsedComboKey = true;

				TextBeforeChange.Emit(InEditingText);

				ClearSelectArea();
				CopyClipboard();

				TextOnChange.Emit(InEditingText);
			}
		}
		else if (KeyMessage->KeyVKCode == 'V')
		{
			if ((GetAsyncKeyState(VK_CONTROL) & 0x8000))
			{
				TextBeforeChange.Emit(InEditingText);

				TextBeforeChange.Emit(InEditingText);

				UsedComboKey = true;

				ClearSelectArea();
				WriteClipboard();

				TextOnChange.Emit(InEditingText);
			}
		}
		else if (KeyMessage->KeyVKCode == VK_UP)
		{
			if (!(GetAsyncKeyState(VK_SHIFT) & 0x8000))
			{
				Caret.CaretTurnUp(LocalTextLayout.Get());
			}
			else
			{
				Caret.CaretSelectionTurnUp(LocalTextLayout.Get());
			}
		}
		else if (KeyMessage->KeyVKCode == VK_DOWN)
		{
			if (!(GetAsyncKeyState(VK_SHIFT) & 0x8000))
			{
				Caret.CaretTurnDown(LocalTextLayout.Get());
			}
			else
			{
				Caret.CaretSelectionTurnDown(LocalTextLayout.Get());
			}
		}
		else if (KeyMessage->KeyVKCode == VK_RIGHT)
		{
			if (!(GetAsyncKeyState(VK_SHIFT) & 0x8000))
			{
				Caret.CaretTurnRight();
			}
			else
			{
				Caret.CaretSelectionTurnRight();
			}
		}
		else if (KeyMessage->KeyVKCode == VK_LEFT)
		{
			if (!(GetAsyncKeyState(VK_SHIFT) & 0x8000))
			{
				Caret.CaretTurnLeft();
			}
			else
			{
				Caret.CaretSelectionTurnLeft();
			}
		}
		else
		{
			return;
		}

		CaretTimer.Start(500);
		ShowCaret = true;

		SetScroller();

		Update();
	}
	if (Message->GetType() == VMessageType::MouseWheelMessage)
	{
		auto MouseWheelMessage = static_cast<VMouseWheelMessage *>(Message);

		if (MouseWheelMessage->MousePosition.InsideRectangle(GetRegion()))
		{
			if (MouseWheelMessage->WheelValue < 0)
			{
				if (!DragResetFontSize)
				{
					OffsetY -= YDelta;

					auto MaxOffset = GetMaxOffsetY();

					if (OffsetY < MaxOffset)
					{
						OffsetY = MaxOffset;
					}
				}
				else
				{
					if ((GetAsyncKeyState(VK_CONTROL) & 0x8000))
					{
						if (Theme->LabelFont->GetTextSize() - 1 >= 12)
						{
							Theme->LabelFont->SetTextSize(Theme->LabelFont->GetTextSize() - 1);
						}
					}
					else
					{
						OffsetY -= YDelta;

						auto MaxOffset = GetMaxOffsetY();

						if (OffsetY < MaxOffset)
						{
							OffsetY = MaxOffset;
						}
					}
				}
			}
			else
			{
				if (!DragResetFontSize)
				{
					OffsetY += YDelta;

					if (OffsetY > 0)
					{
						OffsetY = 0;
					}
				}
				else
				{
					if ((GetAsyncKeyState(VK_CONTROL) & 0x8000))
					{
						if (Theme->LabelFont->GetTextSize() + 1 <= 82)
						{
							Theme->LabelFont->SetTextSize(Theme->LabelFont->GetTextSize() + 1);
						}
					}
					else
					{
						OffsetY += YDelta;

						if (OffsetY > 0)
						{
							OffsetY = 0;
						}
					}
				}
			}
		}

		ResetTextLayout();

		Update();
	}
	if (Message->GetType() == VMessageType::IMECharMessage && UserInOperating)
	{
		auto IMECharMessage = static_cast<VIMECharMessage *>(Message);

		if (UsedComboKey)
		{
			UsedComboKey = false;

			return;
		}

		bool Result = true;
		CheckInput.Emit(IMECharMessage->IMEChar, &Result);

		if (!Result)
		{
			return;
		}

		if (IMECharMessage->IMEChar != L'\b' && IMECharMessage->IMEChar != L'\x13')
		{
			TextBeforeChange.Emit(InEditingText);

			AddCharaceter(IMECharMessage->IMEChar);
		}
		else if (IMECharMessage->IMEChar == L'\b')
		{
			if (FirstKeyPress)
			{
				FirstKeyPress	 = false;
				LastKeyPressTime = time(NULL);

				KeyPressResetTimer.Start(250);
			}

			TextBeforeChange.Emit(InEditingText);

			BackCharacter();
		}

		if (IMECharMessage->IMEChar != L'\x13')
		{
			TextOnChange.Emit(GetPlainText());
			PushNewCharacter.Emit(IMECharMessage->IMEChar);
		}

		Update();
	}
}
} // namespace Core

VLIB_END_NAMESPACE