#ifndef GPTAPIKEYREVIEW_H
#define GPTAPIKEYREVIEW_H

#include <QDialog>
#include <QDateTime>
#include <QTableWidgetItem>

namespace Ui {
class GptApiKeyReview;
}

class GptApiKeyReview : public QDialog
{
    Q_OBJECT

public:
    explicit GptApiKeyReview(QWidget *parent = nullptr);
    ~GptApiKeyReview();

private slots:
    void onCheckBoxStateChanged(int state);
    void onAddButtonClicked();      // 추가 버튼
    void onDeleteButtonClicked();   // 삭제 버튼
    void sortByColumn(int column);  // 컬럼별 정렬

private:
    Ui::GptApiKeyReview *ui;
    bool *sortOrderApiKey;     // API Key 정렬 상태 포인터
    bool *sortOrderRegDate;    // 등록일자 정렬 상태 포인터
    bool *sortOrderRegTime;    // 등록 시간 정렬 상태 포인터

    bool* getSortOrderPointer(int column);  // 컬럼에 맞는 포인터 반환 함수
};

#endif // GPTAPIKEYREVIEW_H
