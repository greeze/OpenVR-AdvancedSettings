
#pragma once

#include <openvr.h>
#include <QtCore/QtCore>
// because of incompatibilities with QtOpenGL and GLEW we need to cherry pick includes
#include <QVector2D>
#include <QMatrix4x4>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QObject>
#include <QSettings>
#include <QQmlEngine>
#include <QtGui/QOpenGLContext>
#include <QtWidgets/QGraphicsScene>
#include <QOffscreenSurface>
#include <QOpenGLFramebufferObject>
#include <QQuickWindow>
#include <QQuickItem>
#include <QQuickRenderControl>
#include <memory>
#include "logging.h"

#include "tabcontrollers/SteamVRTabController.h"
#include "tabcontrollers/ChaperoneTabController.h"
#include "tabcontrollers/MoveCenterTabController.h"
#include "tabcontrollers/FixFloorTabController.h"
#include "tabcontrollers/AudioTabController.h"
#include "tabcontrollers/StatisticsTabController.h"
#include "tabcontrollers/SettingsTabController.h"


// Show a window on the desktop instead of an overlay in vr
//#define DEBUG_DESKTOP_WINDOW


// application namespace
namespace advsettings {

class OverlayController : public QObject {
	Q_OBJECT

public:
	static constexpr const char* applicationKey = "matzman666.AdvancedSettings";
	static constexpr const char* applicationName = "Advanced Settings";
	static constexpr const char* applicationVersionString = "v2.0";

private:
	vr::VROverlayHandle_t m_ulOverlayHandle = vr::k_ulOverlayHandleInvalid;
	vr::VROverlayHandle_t m_ulOverlayThumbnailHandle = vr::k_ulOverlayHandleInvalid;

	std::unique_ptr<QQuickWindow> m_pWindow;
	std::unique_ptr<QOpenGLContext> m_pOpenGLContext;
	std::unique_ptr<QOpenGLFramebufferObject> m_pFbo;
	std::unique_ptr<QQuickRenderControl> m_pRenderControl;
	std::unique_ptr<QOffscreenSurface> m_pOffscreenSurface;

	std::unique_ptr<QTimer> m_pPumpEventsTimer;
	std::unique_ptr<QTimer> m_pRenderTimer;
	bool dashboardVisible = false;

	QPoint m_ptLastMouse;
	Qt::MouseButtons m_lastMouseButtons = 0;

	QUrl m_runtimePathUrl;

	SteamVRTabController steamVRTabController;
	ChaperoneTabController chaperoneTabController;
	MoveCenterTabController moveCenterTabController;
	FixFloorTabController fixFloorTabController;
	AudioTabController audioTabController;
	StatisticsTabController statisticsTabController;
	SettingsTabController settingsTabController;

private:
    OverlayController() : QObject() {}

public:
	virtual ~OverlayController();

	void Init(QQmlEngine* qmlEngine);

	bool isDashboardVisible() {
		return dashboardVisible;
	}

	void SetWidget(QQuickItem* quickItem, const std::string& name, const std::string& key = "");

	void AddOffsetToUniverseCenter(vr::ETrackingUniverseOrigin universe, unsigned axisId, float offset, bool adjustBounds = true, bool commit = true);
	void RotateUniverseCenter(vr::ETrackingUniverseOrigin universe, float yAngle, bool adjustBounds = true, bool commit = true); // around y axis
	void AddOffsetToCollisionBounds(unsigned axisId, float offset, bool commit = true);
	void RotateCollisionBounds(float angle, bool commit = true); // around y axis

	Q_INVOKABLE QString getVersionString();
	Q_INVOKABLE QUrl getVRRuntimePathUrl();

public slots:
	void renderOverlay();
	void OnRenderRequest();
	void OnTimeoutPumpEvents();

	void showKeyboard(QString existingText, unsigned long userValue = 0);

signals:
	void keyBoardInputSignal(QString input, unsigned long userValue = 0);

private:
	static QSettings* _appSettings;
	static std::unique_ptr<OverlayController> singleton;

public:
	static OverlayController* getInstance() {
		if (!singleton) {
			singleton.reset(new advsettings::OverlayController());
		}
		return singleton.get();
	}

	static QSettings* appSettings() { return _appSettings; }

	static void setAppSettings(QSettings* settings) { _appSettings = settings; }
};

} // namespace advsettings
