#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "gptapikeyreview.h"  // GPT API 키 리뷰 UI 클래스 포함

#include <QProcess>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonArray>  // QJsonArray를 처리하기 위한 추가 헤더
#include <QDragEnterEvent>
#include <QDropEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;  // 드래그 앤 드롭 이벤트
    void dropEvent(QDropEvent *event) override;            // 드롭 이벤트 처리

private slots:
    void onUploadBtn();              // 버튼 클릭을 통한 파일 업로드 처리
    void handleReviewProcess(QNetworkReply *reply);      // GPT API 응답 처리
    void onReviewBtnClicked();       // GPT API 호출 버튼 처리

private:
    Ui::MainWindow *ui;
    QStringList fileList;            // 파일 목록 저장
    QNetworkAccessManager *networkManager;  // GPT API 호출을 위한 네트워크 매니저
    GptApiKeyReview *gptApiKeyReviewWindow;  // gptapikeyreview 창을 위한 포인터
    QString loadFileContent(const QString &filePath); // 파일 내용을 읽어옴
};

#endif // MAINWINDOW_H
