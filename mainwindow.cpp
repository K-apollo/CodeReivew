#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QMimeData>
#include <QUrl>
#include <QFile>
#include <QTextStream>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>  // QJsonArray 사용을 위한 헤더 파일 추가
#include <QNetworkReply>

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),                             // 초기화
    networkManager(new QNetworkAccessManager(this)),    // 네트워크 매니저 초기화
    gptApiKeyReviewWindow(nullptr)
{
    ui->setupUi(this);
    setAcceptDrops(true);           // 드래그 앤 드롭 기능 활성화
    connect(ui->uploadButton, &QPushButton::clicked, this, &MainWindow::onUploadBtn);       // 업로드 버튼과 GPT 리뷰 버튼 클릭 이벤트 연결
    connect(ui->reviewButton, &QPushButton::clicked, this, &MainWindow::onReviewBtnClicked);
    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::handleReviewProcess);    // GPT API 응답 처리
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 파일 업로드 버튼 클릭 시 실행되는 함수
void MainWindow::onUploadBtn()
{
    QStringList files = QFileDialog::getOpenFileNames(this, tr("파일 선택"), "", tr("C++ Files (*.cpp *.h);;Python Files (*.py);;Java Files (*.java)"));
    if (files.isEmpty())
        return;

    // 파일 목록을 UI에 추가
    foreach (const QString &file, files) {
        fileList.append(file);
        ui->filePathTextEdit->append(file);  // 줄바꿈하여 추가
    }
}

// 드래그 앤 드롭에서 드래그된 항목을 받아들이는 함수
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();  // 드래그된 것이 파일일 때만 이벤트 수락
    }
}

// 드래그된 파일을 추가하는 함수
void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty())
        return;

    // 드래그된 각 파일의 경로를 추가
    foreach (const QUrl &url, urls) {
        QString fileName = url.toLocalFile();
        if (!fileName.isEmpty()) {
            fileList.append(fileName);
            ui->filePathTextEdit->append(fileName);  // 줄바꿈하여 파일 표시
        }
    }
}

// GPT API 요청을 보내는 함수 (리뷰 버튼 클릭 시 실행)
void MainWindow::onReviewBtnClicked()
{
    // GPT API Key Review 창을 처음 실행할 때 초기화
    if (gptApiKeyReviewWindow == nullptr) {
        gptApiKeyReviewWindow = new GptApiKeyReview(this);
    }

    gptApiKeyReviewWindow->show();  // 창을 보여줌

    if (fileList.isEmpty()) {
        QMessageBox::warning(this, tr("경고"), tr("리뷰할 파일을 업로드하세요."));
        return;
    }

    // 첫 번째 파일의 내용을 읽어서 GPT API로 보냄
    QString fileContent = loadFileContent(fileList.first());

    if (fileContent.isEmpty()) {
        QMessageBox::warning(this, tr("경고"), tr("파일 내용을 읽을 수 없습니다."));
        return;
    }

    // GPT API 요청 생성
    QNetworkRequest request(QUrl("https://api.openai.com/v1/completions"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer YOUR_API_KEY");  // GPT API 키 설정

    // 요청 본문 데이터 (파일 내용을 포함)
    QJsonObject json;
    json["model"] = "text-davinci-003";
    json["prompt"] = "이 코드를 리뷰해 주세요:\n" + fileContent;
    json["max_tokens"] = 1024;

    QJsonDocument jsonDoc(json);
    QByteArray jsonData = jsonDoc.toJson();

    // API 호출
    networkManager->post(request, jsonData);
}

// GPT API 응답 처리 함수
void MainWindow::handleReviewProcess(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObj = jsonDoc.object();

        // GPT API 응답에서 "choices" 배열의 첫 번째 항목을 가져옴
        QJsonArray choicesArray = jsonObj["choices"].toArray();
        QString reviewResult = choicesArray.first().toObject()["text"].toString();

        // 리뷰 결과를 UI에 출력
        ui->reviewOutput->setText(reviewResult);
    } else {
        QMessageBox::warning(this, tr("에러"), tr("GPT API 호출 중 오류가 발생했습니다."));
    }

    reply->deleteLater();
}

// 파일 내용을 로드하는 함수
QString MainWindow::loadFileContent(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return "";
    }

    QTextStream in(&file);
    return in.readAll();
}
