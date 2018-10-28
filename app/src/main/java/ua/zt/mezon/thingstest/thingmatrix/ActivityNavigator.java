package ua.zt.mezon.thingstest.thingmatrix;

public interface ActivityNavigator {
    void buttonEverSetText(String tmp);

    String startEverloop(boolean isEverloopStart);

    void startDemo(int i, int progress, boolean b, int micsamplingFreq);

    void internaleffectsSelectNStart(int i, int progress, boolean b, int micsamplingFreq);
}
