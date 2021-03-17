// Based on the Qt Test tutorial.
// https://doc.qt.io/qt-5/qttestlib-tutorial4-example.html

#include <QtTest>
#include <QCoreApplication>
#include <QtWidgets>
#include "device.h"
//#include "../ui_device.h"


class TestDevice1 : public QObject
{
    Q_OBJECT

public:
    TestDevice1();
    ~TestDevice1();

private slots:
    //void test_case1_data();
    //void test_case1();
    void test_case2_data();
    void test_case2();
};

TestDevice1::TestDevice1()
{}

TestDevice1::~TestDevice1()
{}

/*
void TestDevice1::test_case1_data()
{
    QTest::addColumn<QTestEventList>("events");
    QTest::addColumn<QString>("expected");

    QTestEventList list1;
    list1.addKeyClick('a');
    QTest::newRow("char") << list1 << "a";

    QTestEventList list2;
    list2.addKeyClick('a');
    list2.addKeyClick(Qt::Key_Backspace);
    QTest::newRow("there and back again") << list2 << "";
}

void TestDevice1::test_case1()
{
    QFETCH(QTestEventList, events);
    QFETCH(QString, expected);

    QLineEdit lineEdit;
    Device aDevice;

    events.simulate(&lineEdit);

    QCOMPARE(lineEdit.text(), expected);
}
*/

void TestDevice1::test_case2_data()
{
    QTest::addColumn<QTestEventList>("events");
    QTest::addColumn<bool>("expected");

    Device aDevice;
    QTestEventList list1;
    //QTest::mouseClick(&aDevice,Qt::LeftButton,Qt::NoModifier, QPoint(0,0))
    list1.addMouseClick(Qt::LeftButton);
    QTest::newRow("On") << list1 << true;

    QTestEventList list2;
    list2.addMouseClick(Qt::LeftButton);
    list2.addMouseClick(Qt::LeftButton);
    list2.addDelay(3000);
    QTest::newRow("On and Off!") << list2 << false;
}

void TestDevice1::test_case2()
{
    QFETCH(QTestEventList, events);
    QFETCH(bool, expected);

    Device aDevice;

    events.simulate(aDevice.buttonOn);

    QCOMPARE(aDevice.isPowerOn, expected);
}


QTEST_MAIN(TestDevice1)

#include "tst_device1.moc"
