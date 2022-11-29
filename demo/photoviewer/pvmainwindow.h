#pragma once

#include "../../include/vml/vml.h"
#include "../../include/vml/vmlwidget.h"

struct PVFileReadingProject {
	std::vector<std::wstring> FileList;

	unsigned int LocalViewingFile = 0;

	double LocalScalePercent = 1.f;
};

class VDragbleImageLabel : public Core::VDragControlBase {
private:
	Core::VImage* ViewImage;

public:
	VDragbleImageLabel(Core::VUIObject* Parent, Core::VImage* Image);

	void SetImage(Core::VImage* Image);
	void OnPaint(Core::VCanvasPainter* Painter) override;

	~VDragbleImageLabel() {
		if (ViewImage != nullptr) {
			delete ViewImage;
		}
	}
};

class PVMainWindow : public VML::VMLWidget {
private:
	Core::VImageLabel*			BlurBackgroundImage;
	Core::VImageLabel*			ViewingLabel;

private:
	Core::VPushButton*		MiniSizeButton;
	Core::VPushButton*		MaxSizeButton;
	Core::VPushButton*		CloseButton;

	bool InMaxSize = false;

private:
	Core::VPushButton*		FileOpenButton;

private:
	Core::VUIObject*			MainuiContainer;
	Core::VUIObject*			StartupuiContainer;

private:
	Core::VTextLabel*		TitleText;
	Core::VTextLabel*		PercentTagText;

private:
	Core::VPushButton*		ZoomUpButton;
	Core::VPushButton*		ZoomDownButton;
	Core::VPushButton*		OneToOneButton;

private:
	Core::VDragControlBase* ImageDragger;
	Core::VScaleLayout*	    ImageDraggerLayout;
							
private:				  
	PVFileReadingProject*	ViewingProject;
	Core::VImage*			ViewingImage;

protected:
	void MiniSizeButtonOnClicked();
	void MaxSizeButtonOnClicked();
	void CloseButtonOnClicked();

	void ZoomUpButtonOnClicked();
	void ZoomDownButtonOnClicked();
	void OneToOneButtonOnClicked();

	void WindowOnResize(const int&, const int&);

protected:
	Core::VPoint OffsetPoint;
	Core::VPoint CenterOffsetPoint;

	bool StartDrag = false;

	void ImageStartDrag();
	void ImageOnDrag(const int& X, const int &Y);

private:
	std::wstring GetFileName(const std::wstring& FilePath);
	std::wstring GetFolderPath(const std::wstring& FilePath);
	std::wstring GetPercentString(const double& ScalePercent);

private:
	void CreateViewingProject(const std::wstring& FilePath);

private:
	void OpenViewProject();

public:
	void OnMessage(Core::VMessage* Message) override;

public:
	PVMainWindow(Core::VApplication* Parent);
	PVMainWindow(Core::VApplication* Parent, const std::wstring& FilePath);
};