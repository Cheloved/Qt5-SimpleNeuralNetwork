#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QWidget>
#include <QImage>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>

class ScribbleArea : public QWidget
{
    Q_OBJECT
public:
    explicit ScribbleArea(QWidget *parent = nullptr);

    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);

    bool isModified() const { return this->modified; }
    QColor getPenColor() const { return penColor; }
    int getPenWidth() const { return penWidth; }

    QImage image;

public slots:
    void clearArea();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    bool modified;
    bool scribbling;
    int penWidth;
    QColor penColor;
    QPoint lastPoint;

    void DrawLineTo(const QPoint &endPoint);
    void resizeImage(QImage *image, const QSize &newSize);
};

#endif // SCRIBBLEAREA_H
