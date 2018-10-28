package ua.zt.mezon.thingstest.thingmatrix;

public interface MainActivityPresenter {
    void matrixEverloopOperationStart(int i, boolean isEverloopStart);

    void matrixDemoOperationStart(int i, int progress, boolean b, int efftype);
}
