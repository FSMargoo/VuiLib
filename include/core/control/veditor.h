#pragma once

#include "../object/vuiobject.h"
#include "../uibasic/vsmarttimer.h"
#include "vabstractbutton.h"

#include <map>

VLIB_BEGIN_NAMESPACE

namespace Core {
	enum class VEditorCaretSelectMode {
		Left, Right, Up, Down
	};
	struct VEditorCaret {
		std::wstring*		   CacheTargetPtr;

		bool				   InSelecting;

		VEditorCaretSelectMode SelectMode;

		int					   CaretStart;
		int					   CaretEnd;
							   
		void				   CaretSelectionTurnUp(IDWriteTextLayout* TextLayout);
		void				   CaretSelectionTurnDown(IDWriteTextLayout* TextLayout);
		void				   CaretSelectionTurnLeft();
		void				   CaretSelectionTurnRight();

		void				   ResetSelect();

		void				   CaretSelectionTurnLineHead(IDWriteTextLayout* TextLayout);
		void				   CaretSelectionTurnLineEnd(IDWriteTextLayout* TextLayout);

		void				   CaretSelectAll();

		void				   CaretTurnLeft();
		void				   CaretTurnRight();
		void				   CaretTurnUp(IDWriteTextLayout* TextLayout);
		void				   CaretTurnDown(IDWriteTextLayout* TextLayout);

		void				   CaretTurnLineHead(IDWriteTextLayout* TextLayout);
		void				   CaretTurnLineEnd(IDWriteTextLayout* TextLayout);

		void				   CaretPageUp();
		void				   CaretPageEnd();

		void				   SetValidCaretRange();

		void				   SetCaretByMousePosition(const int& X, const int& Y, IDWriteTextLayout* TextLayout);
		void				   SetCaretSelectionByMousePosition(const int& X, const int& Y, IDWriteTextLayout* TextLayout);

		VPoint				   GetCaretPosition(IDWriteTextLayout* TextLayout);
		DWRITE_TEXT_RANGE	   GetTextRange();
	};

	class VEditor : public VAbstractButton {
	public:
		VEditor(VUIObject* Parent);
		VEditor(const int& Width, const int& Height, VUIObject* Parent);

	public:
		void OnPaint(VCanvasPainter* Painter) override;

	public:
		void SetPlaneText(const std::wstring& PlaneText);
		std::wstring GetPlaneText() const;

	public:
		void ResetCaretStatus();
		void ResetTextLayout();

		bool GetAllowEditStatus();
		void SetAllowEditStatus(const bool& Status);
		
		void ScrollToEnd();

	public:
		void LeftClickedDown() override;
		void GotMouseFocus() override;
		void OnMessage(VMessage* Message) override;

		void Resize(const int& Width, const int& Height) override;

	public:
		void CheckFrame() override;

	public:
		void SetDeltaY(const int& Delta);

	public:
		VTextEditorTheme* GetTheme();

	public:
		VSignal<const std::wstring&> TextOnChange;

	private:
		bool ClearSelectArea();
		void BackCharacter();
		void DeleteCharacter();
		void AddCharaceter(const wchar_t& Character);

	private:
		int GetMaxOffsetY();

	private:
		void SetScroller();

	private:
		bool IsASCIICharacter(const wchar_t& ASCIICode);

	private:
		void CopyClipboard();
		void WriteClipboard();

	public:
		std::vector<std::tuple<ID2D1Effect*, DWRITE_TEXT_RANGE>> TextEffect;

	private:
		VSmartTimer								  CaretTimer;
		bool									  ShowCaret;
		bool									  UserInOperating;
		bool									  FirstBackOff;

		long int								  OffsetY;
		long int								  OffsetX;

		std::wstring							  InEditingText;
		VTextEditorTheme*						  Theme;

		VEditorCaret							  Caret;

		Microsoft::WRL::ComPtr<IDWriteTextLayout> LocalTextLayout;

	private:
		bool InMouseDragSelecting;
		bool UsedComboKey;

	private:
		VBasicTimer             AnimationFrameTimer;
		VBasicTimer				BackoffTimer;
		VBasicTimer				BackoffResetTimer;
		VAnimationInterpolator* Interpolator;

		time_t					LastBackOffTime;
		int						YDelta;

		VLabelStatusTheme       OldTheme;
		VLabelStatusTheme       TargetTheme;

		bool					AllowEdit;

		bool                    InAnimation = false;
	};
}

VLIB_END_NAMESPACE