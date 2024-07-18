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
 * \file vMonitor.h
 * \brief The monitor wrapper of GLFW API
 */

#pragma once

#include <include/base/event/vEvent.h>
#include <include/widget/vGLFWException.h>

#include <glfw/glfw3.h>

/**
 * The status for the monitor class
 *
 * When the monitor connect, it will be a valid monitor
 * instance, otherwise invalid
 */
enum class VMonitorStatus {
	Valid,
	Invalid
};

/**
 * This class is a wrapper of GLFW monitor API, which will
 * maintain a GLFWmonitor pointer. When a monitor was connected
 * the instance of the monitor will be marked as valid, otherwise invalid.
 */
class VMonitor {
public:
	/**
	 * Alias of int, used by the GLFW API
	 */
	using MonitorEventType = int;

public:
	/**
	 * Create a monitor class by the monitor index
	 * @param Index The index of the monitor, depending on the OS
	 */
	explicit VMonitor(const int &Index);
	/**
	 * By default, VMonitor will create the primary monitor instance
	 */
	VMonitor();

public:
	/**
	 * Get the status of the monitor
	 * @return If the monitor is connected or created properly, it will return
	 * VMonitorStatus::Valid otherwise VMonitorStatus::Invalid
	 */
	[[nodiscard]] const VMonitorStatus GetStatus() const;

private:
	/**
	 * Function for GLFW monitor callback, it will emit the event
	 * _monitorOnEvent to spread message to each instance of VMonitor
	 * @param Monitor Provided by GLFW
	 * @param Event Provided by GLFW
	 */
	static void GLFWMonitorCallback(GLFWmonitor *Monitor, MonitorEventType Event) {
		_monitorOnEvent.Emit(Monitor, Event);
	}
	/**
	 * The actual callback of the monitor event, when the monitor connected
	 * this monitor instance will be marked as valid, otherwise invalid.
	 * @param Monitor The monitor GLFW API pointer
	 * @param Event The event type
	 */
	void OnMonitor(GLFWmonitor *Monitor, MonitorEventType Event);

private:
	/**
	 * The event to convert GLFW message into VUILib's message bus
	 */
	static VEvent<GLFWmonitor *, MonitorEventType> _monitorOnEvent;

private:
	VMonitorStatus _status;
	GLFWmonitor	  *_glfwMonitor;
};