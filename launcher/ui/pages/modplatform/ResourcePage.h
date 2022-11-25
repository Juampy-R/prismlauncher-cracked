#pragma once

#include <QTimer>
#include <QWidget>

#include "modplatform/ModIndex.h"
#include "modplatform/ResourceAPI.h"

#include "ui/pages/BasePage.h"
#include "ui/widgets/ProgressWidget.h"

namespace Ui {
class ResourcePage;
}

class BaseInstance;
class ResourceModel;
class ResourceDownloadDialog;

class ResourcePage : public QWidget, public BasePage {
    Q_OBJECT
   public:
    ~ResourcePage() override;

    /* Affects what the user sees */
    [[nodiscard]] auto displayName() const -> QString override = 0;
    [[nodiscard]] auto icon() const -> QIcon override = 0;
    [[nodiscard]] auto id() const -> QString override = 0;
    [[nodiscard]] auto helpPage() const -> QString override = 0;
    [[nodiscard]] bool shouldDisplay() const override = 0;

    /* Used internally */
    [[nodiscard]] virtual auto metaEntryBase() const -> QString = 0;
    [[nodiscard]] virtual auto debugName() const -> QString = 0;

    [[nodiscard]] virtual inline QString resourceString() const { return tr("resource"); }

    /* Features this resource's page supports */
    [[nodiscard]] virtual bool supportsFiltering() const = 0;

    void retranslate() override;
    void openedImpl() override;
    auto eventFilter(QObject* watched, QEvent* event) -> bool override;

    /** Get the current term in the search bar. */
    [[nodiscard]] auto getSearchTerm() const -> QString;
    /** Programatically set the term in the search bar. */
    void setSearchTerm(QString);

    [[nodiscard]] bool isPackSelected(const ModPlatform::IndexedPack&, int version = -1) const;
    [[nodiscard]] auto getCurrentPack() const -> ModPlatform::IndexedPack;

    [[nodiscard]] auto getDialog() const -> const ResourceDownloadDialog* { return m_parent_dialog; }

   protected:
    ResourcePage(ResourceDownloadDialog* parent, BaseInstance&);

   public slots:
    virtual void updateUi();
    virtual void updateSelectionButton();
    virtual void updateVersionList();

    virtual void addResourceToDialog(ModPlatform::IndexedPack&, ModPlatform::IndexedVersion&);
    virtual void removeResourceFromDialog(ModPlatform::IndexedPack&, ModPlatform::IndexedVersion&);

   protected slots:
    virtual void triggerSearch() {}
   
    void onSelectionChanged(QModelIndex first, QModelIndex second);
    void onVersionSelectionChanged(QString data);
    void onResourceSelected();

    /** Associates regex expressions to pages in the order they're given in the map. */
    [[nodiscard]] virtual QMap<QString, QString> urlHandlers() const = 0;
    virtual void openUrl(const QUrl&);

    /** Whether the version is opted out or not. Currently only makes sense in CF. */
    virtual bool optedOut(ModPlatform::IndexedVersion& ver) const { return false; };

   public:
    BaseInstance& m_base_instance;

   protected:
    Ui::ResourcePage* m_ui;

    ResourceDownloadDialog* m_parent_dialog = nullptr;
    ResourceModel* m_model = nullptr;

    int m_selected_version_index = -1;

    ProgressWidget m_fetch_progress;

    // Used to do instant searching with a delay to cache quick changes
    QTimer m_search_timer;
};
