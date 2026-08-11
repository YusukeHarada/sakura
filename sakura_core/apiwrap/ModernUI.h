/*! @file
	@brief モダンUI(Windows 11 世代の外観)ヘルパー

	モダンUI設定を darkmodelib の設定へ反映する。

	角丸・Mica・境界色・キャプション色の DwmSetWindowAttribute は
	darkmodelib の DarkMode::setDarkTitleBarEx() が一手に引き受けており、
	何を適用するかは同ライブラリのグローバル設定で決まる。
	サクラエディタはこれまでその設定を触っていなかったため、
	既定値 (DWMWCP_DEFAULT / DWMSBT_AUTO) のままで角丸も Mica も
	無効になっていた。

	そこで自前で DwmSetWindowAttribute を呼ぶのではなく、
	ライブラリ側の設定を切り替える。自前で呼ぶと
	setDarkTitleBarEx() が実行されるたびに上書きされてしまう。

	SPDX-License-Identifier: Zlib
*/
#pragma once

#include <dwmapi.h>

#include "apiwrap/DarkMode.h"

namespace ModernUI {

/*! モダンUI設定を darkmodelib に反映する

	設定後に DarkMode::setDarkTitleBarEx() を呼ぶと、実際のウィンドウに
	適用される。setDarkTitleBarEx() 側で Windows 11 判定
	(isAtLeastWindows11 / ビルド番号 22621) を行っているため、
	古い OS では何も起こらず従来の外観が保たれる。

	@param bModernUI  共有データの m_bModernUI 値
*/
inline void ApplyModernUISetting(BOOL bModernUI)
{
	// 角丸
	DarkMode::setRoundCornerConfig(bModernUI ? DWMWCP_ROUND : DWMWCP_DEFAULT);

	// Mica
	// 編集領域 (CEditView) は背景を不透明に塗り潰すので、
	// 実際に Mica が見えるのはタイトルバー周辺に限られる。
	// クライアント領域まで広げる setMicaExtendedConfig() は使わない。
	DarkMode::setMicaConfig(bModernUI ? DWMSBT_MAINWINDOW : DWMSBT_AUTO);
}

}	// namespace ModernUI
