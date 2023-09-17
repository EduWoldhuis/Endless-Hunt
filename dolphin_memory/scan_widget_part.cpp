/*
void MemScanWidget::onFirstScan()
{
    m_memScanner->setType(static_cast<Common::MemType>(m_cmbScanType->currentIndex()));
    m_memScanner->setIsSigned(m_chkSignedScan->isChecked());
    m_memScanner->setEnforceMemAlignement(m_chkEnforceMemAlignement->isChecked());
    m_memScanner->setBase(static_cast<Common::MemBase>(m_btnGroupScanBase->checkedId()));
    Common::MemOperationReturnCode scannerReturn =
        m_memScanner->firstScan(getSelectedFilter(), m_txbSearchTerm1->text().toStdString(),
            m_txbSearchTerm2->text().toStdString());
    if (scannerReturn != Common::MemOperationReturnCode::OK)
    {
        handleScannerErrors(scannerReturn);
    }
    else
    {
        int resultsFound = static_cast<int>(m_memScanner->getResultCount());
        m_lblResultCount->setText(
            tr("%1 result(s) found", "", resultsFound).arg(QString::number(resultsFound)));
        if (resultsFound <= 1000 && resultsFound != 0)
        {
            m_btnAddAll->setEnabled(true);
            m_btnAddSelection->setEnabled(true);
            m_btnRemoveSelection->setEnabled(true);
        }
        m_btnFirstScan->hide();
        m_btnNextScan->show();
        m_btnResetScan->show();
        m_cmbScanType->setDisabled(true);
        m_chkSignedScan->setDisabled(true);
        m_chkEnforceMemAlignement->setDisabled(true);
        m_groupScanBase->setDisabled(true);
        updateScanFilterChoices();
    }
}
*/