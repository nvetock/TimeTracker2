# Simple Time Tracker

## Description
A lightweight time tracker application built to make life simple when it comes to tracking and documenting time spent during the work day.

The application is written in C++ with the Qt framework.

This app is designed to be cross-platform, keeps a history of projects overtime and allows CSV exporting.

A future state may function with Asana and maybe other project mgmt products.

### Project Goal & Why
After a short search I found most time trackers are over-engineered and costly. I needed something simple and straight-forward without all the bells and whistles of a time tracker that is also a bird, a plane and a project management system all-in-one.\
The other solution was manual time-entry and who has time for that?
### Current Roadmap
- A logger class to support the beginning of this built
- Functioning data objects built
- A system that tracks time accurately (complete)
- A way to display time tracking to console. (complete)
- A way to interact with: start, pause, stop. the timer from console (complete-ish, moving manual intervention to button UI)
- Idle tracking state that will auto-pause any further progress on timer. (complete)
- A UI that supports multiple pages with similar styling
- Defined StyleSheet classes that can be used universally
- Set Color Theme (complete-ish for now)
- UI Connected to backend timing and other program features.
- Data persistence.
### Pages Coming
- HomePage
- Log History
- Export Page
- TrackWorkTimerPage.cpp
- TrackWorkSetupPage.cpp
- LogHistoryPage.cpp
- SettingsPage.cpp
## Architecture
### Domain
- WorkSession
- UserPreferences
### Infrastructure
- IIdleDetector
- CsvLogger
### Application Services
- AppController
- SessionManager
- ActivityMonitor
- ReminderScheduler
### UI
- MainWindow
- TrayIconController
- ReminderDialog

---

## Future Roadmap
- Modern UX/UI
- Integration to pull from Asana
- 'Offline' Async working mode that is capable of storing information locally allowing users to have less windows open.
- Allow for a more 'universal' timer that can attach itself to different project management APIs
