<distribution version="13.0.0" name="Nav_CVI" type="MSI">
	<prebuild>
		<workingdir>workspacedir</workingdir>
		<actions></actions></prebuild>
	<postbuild>
		<workingdir>workspacedir</workingdir>
		<actions></actions></postbuild>
	<msi GUID="{6769D334-1053-43AD-8D04-9F0BA8CAE13C}">
		<general appName="Nav_CVI" outputLocation="c:\Users\stud_332\Desktop\NAV_VIZ_CVI\cvidistkit.Nav_CVI" relOutputLocation="cvidistkit.Nav_CVI" outputLocationWithVars="c:\Users\stud_332\Desktop\NAV_VIZ_CVI\cvidistkit.%name" relOutputLocationWithVars="cvidistkit.%name" upgradeBehavior="1" autoIncrement="true" version="1.0.3">
			<arp company="KTU" companyURL="" supportURL="" contact="" phone="" comments=""/>
			<summary title="" subject="" keyWords="" comments="" author=""/></general>
		<userinterface language="English" showPaths="true" readMe="" license="">
			<dlgstrings welcomeTitle="Nav_CVI" welcomeText=""/></userinterface>
		<dirs appDirID="100">
			<installDir name="Nav_CVI" dirID="100" parentID="2" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="[Start&gt;&gt;Programs]" dirID="7" parentID="-1" isMSIDir="true" visible="true" unlock="false"/>
			<installDir name="Nav_CVI" dirID="101" parentID="7" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="[Program Files]" dirID="2" parentID="-1" isMSIDir="true" visible="true" unlock="false"/></dirs>
		<files>
			<simpleFile fileID="0" sourcePath="c:\Users\stud_332\Desktop\NAV_VIZ_CVI\cvibuild.Nav_CVI\Release\Nav_CVI.exe" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="1" sourcePath="c:\Users\stud_332\Desktop\NAV_VIZ_CVI\Nav_CVI.uir" relSourcePath="Nav_CVI.uir" relSourceBase="0" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="2" sourcePath="c:\Users\stud_332\Desktop\NAV_VIZ_CVI\AtUsbHid.dll" relSourcePath="AtUsbHid.dll" relSourceBase="0" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/></files>
		<fileGroups>
			<projectOutput targetType="0" dirID="100" projectID="0">
				<fileID>0</fileID></projectOutput>
			<projectDependencies dirID="100" projectID="0">
				<fileID>2</fileID></projectDependencies></fileGroups>
		<shortcuts>
			<shortcut name="Nav_CVI" targetFileID="0" destDirID="101" cmdLineArgs="" description="" runStyle="NORMAL"/></shortcuts>
		<mergemodules/>
		<products/>
		<runtimeEngine installToAppDir="false" activeXsup="true" analysis="true" cvirte="true" dotnetsup="true" instrsup="true" lowlevelsup="true" lvrt="true" netvarsup="true" rtutilsup="true">
			<hasSoftDeps/></runtimeEngine><sxsRuntimeEngine>
			<selected>false</selected>
			<doNotAutoSelect>false</doNotAutoSelect></sxsRuntimeEngine>
		<advanced mediaSize="650">
			<launchConditions>
				<condition>MINOS_WINXP</condition>
			</launchConditions>
			<includeConfigProducts>true</includeConfigProducts>
			<maxImportVisible>silent</maxImportVisible>
			<maxImportMode>merge</maxImportMode>
			<custMsgFlag>false</custMsgFlag>
			<custMsgPath>msgrte.txt</custMsgPath>
			<signExe>false</signExe>
			<certificate></certificate>
			<signTimeURL></signTimeURL>
			<signDescURL></signDescURL></advanced>
		<Projects NumProjects="1">
			<Project000 ProjectID="0" ProjectAbsolutePath="c:\Users\stud_332\Desktop\NAV_VIZ_CVI\Nav_CVI.prj" ProjectRelativePath="Nav_CVI.prj"/></Projects>
		<buildData progressBarRate="1.603400999514486">
			<progressTimes>
				<Begin>0.000000000000000</Begin>
				<ProductsAdded>0.063007375000000</ProductsAdded>
				<DPConfigured>3.425508125000000</DPConfigured>
				<DPMergeModulesAdded>4.123006875000000</DPMergeModulesAdded>
				<DPClosed>6.923006874999999</DPClosed>
				<DistributionsCopied>8.148094124999998</DistributionsCopied>
				<End>62.367430249999998</End></progressTimes></buildData>
	</msi>
</distribution>
