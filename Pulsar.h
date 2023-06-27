#ifndef PULSAR_H_
#define PULSAR_H_

#include <wx/wx.h>

#include <chrono>
#include <stdexcept>
#include <functional>

#include"MyMacros.h"

class Pulsar : public wxEvtHandler {

public:
	CLASS_CONSTRUCT(Pulsar)() { m_timer.SetOwner(this), Bind(wxEVT_TIMER, &Pulsar::OnTimer, this); }
	CLASS_DESTRUCT(Pulsar)() {}
    void SetOnIteration(const std::function<void()>& onIter) { m_onIter = onIter; }
	void SetOnStop(const std::function<void()>& onStop) { m_onStop = onStop; }

	void Start(
		size_t durationSecOfIter = 1
		, bool oneShot = wxTIMER_CONTINUOUS
	) {
		if (durationSecOfIter <= 0) throw std::runtime_error("Duration must be positive");

		m_startTime = std::chrono::steady_clock::now();
		m_timer.Start(durationSecOfIter * 1000, oneShot);
	}
	
	void Stop(
	) {
		m_timer.Stop();
		m_elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_startTime).count();
		m_onStop();	
	}
	
	bool IsRunning() { return m_timer.IsRunning(); }
private:
	wxTimer m(timer);

	std::function<void()> m(onIter), m(onStop);

    std::chrono::steady_clock::time_point m(startTime);
	int64_t m(elapsedMs);

	void OnTimer(wxTimerEvent& evt) { m_onIter(); }
};

#endif // !PULSAR_H_