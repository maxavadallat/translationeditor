import QtQuick 2.0

Loader {
    id: loaderRoot

    property bool queued: false
    property int lastStatus: -1

    property bool fadeInWhenLoaded: true
    property bool deactiveteOnFadeOut: true

    property string newSource: ""
    property bool loadNewSourceWhenFinished: false

//    // Loaders Count
//    property int loadersCount: mainController.loadersCount
    // Loaders Count Max
    readonly property int loadersCountMax: 4

    // Active State - Must Not Set Directly! Use activate/deactivate
    active: false
    asynchronous: true

    signal fadeInFinished()
    signal fadeOutFinished()

    onActiveChanged: {
        //console.log("TELoader.onActiveChanged - active: " + active);

        // ...
    }

//    onLoadersCountChanged: {
//        console.log("TELoader.onLoadersCountChanged - mainController.loadersCount: " + mainController.loadersCount);

//        // Check If Queued & Loaders Count
//        if (loaderRoot.queued && mainController.loadersCount < loaderRoot.loadersCountMax) {
//            // Activate

//            // Reset Queued State
//            loaderRoot.queued = false;
//            // Set Active State
//            loaderRoot.active = true;
//        }
//    }

    onNewSourceChanged: {
        //console.log("TELoader.onNewSourceChanged - newSource: " + newSource);

        // Set Load New Source When Finished
        loaderRoot.loadNewSourceWhenFinished = true;

        // Check If Active
        if (loaderRoot.active) {
            // Fade Out
            fadeOut();
        } else {
            // Set Source
            loaderRoot.source = newSource;
            // Activate
            loaderRoot.activate();
        }
    }

    onStatusChanged: {
        //console.log("TELoader.onStatusChanged - status: " + status);

        // Check Last Status
        if (loaderRoot.lastStatus !== loaderRoot.status) {
            // Set Last Status
            loaderRoot.lastStatus = loaderRoot.status;
        }

        // Check Status
        if (loaderRoot.status === Loader.Error) {
            console.error("TELoader.onStatusChanged - error loading component: " + loaderRoot.sourceComponent.errorString());

            return;
        }

        // Check Load New Source
        if (loaderRoot.loadNewSourceWhenFinished) {
            //console.log("TELoader.onStatusChanged - status: " + loaderRoot.status);
            // Reset Load New Sourcce When Finished
            loaderRoot.loadNewSourceWhenFinished = false;

            // Restart Change Source Timer
            changeSourceTimer.restart();
        }
    }

    onLastStatusChanged: {
        //console.log("TELoader.onLastStatusChanged - status: " + lastStatus + " - source: " + source);

        switch (lastStatus) {
            case Loader.Loading:
                mainController.loadersCount++;
            break;

            case Loader.Ready:
                mainController.loadersCount--;
            break;

            case Loader.Error:
                mainController.loadersCount--;
            break;
        }
    }

    onLoaded: {
        // Check Fade In When Loaded
        if (loaderRoot.fadeInWhenLoaded && loaderRoot.opacity < 1.0) {
            // Fade In
            loaderRoot.fadeIn();
        }
    }

    onFadeOutFinished: {
        // Check Deactivate On Fade Out
        if (loaderRoot.deactiveteOnFadeOut) {
            // Set Active State
            loaderRoot.active = false;
        }
    }

    // Activate Loader
    function activate() {
        // Check If Active
        if (!loaderRoot.active) {
            // Check Loaders Count
            if (mainController.loadersCount < loaderRoot.loadersCountMax) {
                // Set Last Status
                loaderRoot.lastStatus = Loader.Loading;

                // Set Queued State
                loaderRoot.queued = false;
                // Set Active State
                loaderRoot.active = true;
            } else {
                // Set Queued State
                loaderRoot.queued = true;
            }
        }
    }

    // Deactivate
    function deactivate() {
        // Check If Active
        if (loaderRoot.active) {
            // Fade Out
            loaderRoot.fadeOut();
        }
    }

    // Fade In
    function fadeIn() {
        //console.log("TELoader.fadeIn");
        // Start Fade In Animation
        fadeInAnim.start();
    }

    // Fade Out
    function fadeOut() {
        //console.log("TELoader.fadeOut");
        // Start Fade Out Animation
        fadeOutAnim.start();
    }

    // Change Source Timer
    Timer {
        id: changeSourceTimer
        interval: 10

        onTriggered: {
            // Check Status & New Source
            if (loaderRoot.status === Loader.Null && loaderRoot.newSource !== "") {
                // Set Active State
                loaderRoot.activate();
                // Set Source
                loaderRoot.source = loaderRoot.newSource;
            }
        }
    }

    // System Model Connections
    Connections {
        target: mainController

        onLoadersCountChanged: {
            //console.log("TELoader.mainController.onLoadersCountChanged - loadersCount: " + mainController.loadersCount);

            // Check If Queued & Loaders Count
            if (loaderRoot.queued && (mainController.loadersCount < loaderRoot.loadersCountMax)) {
                // Set Last Status
                loaderRoot.lastStatus = Loader.Loading;
                // Reset Queued State
                loaderRoot.queued = false;
                // Set Active State
                loaderRoot.active = true;

                // ...
            }
        }
    }

    // Fade Out Animation
    TEFade {
        id: fadeOutAnim
        alwaysRunToEnd: true
        target: loaderRoot
        to: 0.0
        onStopped: {
            //console.log("TELoader.fadeOutAnim.onStopped");
            // Emit Fade Out Finished Signal
            fadeOutFinished();
        }
    }

    // Fade In Animation
    TEFade {
        id: fadeInAnim
        alwaysRunToEnd: true
        target: loaderRoot
        to: 1.0
        onStopped: {
            //console.log("TELoader.fadeInAnim.onStopped");
            // Emit Fade Out Finished Signal
            fadeInFinished();
        }
    }
}
