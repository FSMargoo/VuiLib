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
		void SetLeadingText(const std::wstring& Text);
		std::wstring GetLedingText() const;

	public:
		void ResetCaretStatus();
		void ResetTextLayout();

		bool GetAllowEditStatus() const;
		void SetAllowEditStatus(const bool& Status);

		bool GetAllowFontSizeDragStatus() const;
		void SetAllowFontSizeDragStatus(const bool& Status);

		bool GetOperationBackStatus() const;
		void SetOperationBackStatus(const bool& Status);

		int	 GetMaxOperationCache() const;
		void SetMaxOperationCache(const int& Value);
				
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
		VEditorCaret GetCaret();
		void		 SetCaret(const VEditorCaret& CaretValue);

	public:
		IDWriteTextLayout* GetTextLayout();

	public:
		VSignal<const std::wstring&>	TextOnChange;
		VSignal<const std::wstring&>	TextBeforeChange;
		VSignal<const wchar_t&>			PushNewCharacter;
		VSignal<const wchar_t&, bool*>	CheckInput;
		VSignal<const int&>				TextClicked;

	private:
		void InitEditor();

	protected:
		void ResetOffsetYByCaret();

	protected:
		bool ClearSelectArea();
		void BackCharacter();
		void DeleteCharacter();
		void AddCharaceter(const wchar_t& Character);

	protected:
		int GetMaxOffsetY();

	protected:
		void SetScroller();

	protected:
		bool IsASCIICharacter(const wchar_t& ASCIICode);

	protected:
		void CopyClipboard();
		void WriteClipboard();

	public:
		std::vector<std::tuple<ID2D1Effect*, DWRITE_TEXT_RANGE>>		TextEffect;
		std::vector<std::tuple<DWRITE_FONT_STYLE, DWRITE_TEXT_RANGE>>	TextStyle;
		std::vector<DWRITE_TEXT_RANGE>									UnderlineStyle;

	protected:
		VSmartTimer								  CaretTimer;
		bool									  ShowCaret;
		bool									  UserInOperating;
		bool									  FirstKeyPress;

		bool									  DragResetFontSize;

		long int								  OffsetY;
		long int								  OffsetX;

		std::wstring							  InEditingText;
		VTextEditorTheme*						  Theme;

		VEditorCaret							  Caret;

		Microsoft::WRL::ComPtr<IDWriteTextLayout> LocalTextLayout;

	private:
		void DrawSelectedRange(Core::VCanvasPainter* Painter);

	private:
		void CacheOperation(const std::wstring& OldText);

	protected:
		bool InMouseDragSelecting;
		bool UsedComboKey;

	protected:
		VBasicTimer					AnimationFrameTimer;
		VBasicTimer					KeyPressTimer;
		VBasicTimer					KeyPressResetTimer;
		VAnimationInterpolator*		Interpolator;

		time_t						LastKeyPressTime;
		int							YDelta;

		VLabelStatusTheme			OldTheme;
		VLabelStatusTheme			TargetTheme;

		bool						AllowEdit;

		bool						InAnimation;

	protected:
		bool						EnableOperationBack;
		int							OperationCacheMax;
		std::vector<VEditorCaret>	CaretSet;
		std::vector<std::wstring>	OldStringSet;

	protected:
		std::wstring LeadText;
	};
}

VLIB_END_NAMESPACE