package ua.zt.mezon.thingstest.thingmatrix;

import android.os.Handler;
import android.os.HandlerThread;

import com.don11995.log.SimpleLog;

public class MainActivityPresenterImpl implements MainActivityPresenter {
    private final ActivityNavigator mActivityNavigator;

    HandlerThread mBackgroundThread = new HandlerThread("MyBackgroundThread");
    Handler matrixOperationHandler = null;
    private boolean isEverloopStart;

    public MainActivityPresenterImpl(ActivityNavigator activityNavigator) {
        mActivityNavigator = activityNavigator;
        mBackgroundThread.start();
        matrixOperationHandler = new Handler(mBackgroundThread.getLooper());
    }

    private final Runnable turnOnEverloop = new Runnable() {
        @Override
        public void run() {
            mActivityNavigator.buttonEverSetText(mActivityNavigator.startEverloop(isEverloopStart));
        }
    };


    @Override
    public void matrixEverloopOperationStart(int i, boolean isEverloopStart) {
        this.isEverloopStart = isEverloopStart;
        matrixOperationHandler.post(turnOnEverloop);
        SimpleLog.v("TODEL matrixEverloopOperationStart ");
    }

    @Override
    public void matrixDemoOperationStart(int i, int progress, boolean b, int freq) {
        Runnable turnOnDemo = new Runnable() {
            @Override
            public void run() {
                mActivityNavigator.internaleffectsSelectNStart(i, progress, b, 16000);
                ;
            }
        };
        matrixOperationHandler.post(turnOnDemo);

    }
}
