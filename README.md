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
- ~~Functioning data objects built~~
- ~~A system that tracks time accurately (complete)~~
- ~~A way to display time tracking to console. (complete)~~
- ~~A way to interact with: start, pause, stop. the timer from console (complete-ish, moving manual intervention to button UI)~~
- ~~Idle tracking state that will auto-pause any further progress on timer. (complete)~~
- ~~A UI that supports multiple pages with similar styling~~
- ~~Defined StyleSheet classes that can be used universally~~
- ~~Set Color Theme (complete-ish for now)~~
- ~~UI Connected to backend timing and other program features.~~
- ~~Data persistence.~~
- Add 'Log History' page for quick user views into log data
- Establish better UX flow between pages. Specifically the buttons on the Time tracking page
- Add a 'Start new task' or 'main menu' button on task completion
- Add a UI component that signifies an export was/wasn't successful
- Add ability to overwrite file on export rather than cancel the export
- Overhaul UI and styling components of general layout and buttons to a unified and better designed experience.
- Bug testing: Can this work on multi-monitor setup? And can it be swapped to different monitors?
- QOL feature: Is there additional information that should be added to the WorkSession?
- QOL feature: Is there a customizable 'add new column' that lets a user define a custom ComboBox

### Pages Coming
- ~~HomePage~~
- Log History
- ~~Export Page~~
- ~~TrackWorkTimerPage.cpp~~
- ~~TrackWorkSetupPage.cpp~~
- ~~LogHistoryPage.cpp~~
- ~~SettingsPage.cpp~~

---
## Architecture
### Domain
- WorkSession
- UserPreferences
### Infrastructure
- IIdleDetector
- AppSettingsRepo
- SessionLogRepo
### Application Services
- AppController
- SessionManager
- ActivityMonitor
- ReminderScheduler
### UI
- BaseCard

---

## Future Roadmap
- Modern UX/UI
- Integration to pull from Asana and other PjMgmt Software
- Allow for a more 'universal' timer that can attach itself to different project management APIs
- Animated nudges that modernize the current idle states
