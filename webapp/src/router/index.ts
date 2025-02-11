import AboutView from '@/views/AboutView.vue';
import BatteryAdminView from '@/views/BatteryAdminView.vue';
import AcChargerAdminView from '@/views/AcChargerAdminView.vue';
import ConfigAdminView from '@/views/ConfigAdminView.vue';
import ConsoleInfoView from '@/views/ConsoleInfoView.vue';
import DeviceAdminView from '@/views/DeviceAdminView.vue';
import DtuAdminView from '@/views/DtuAdminView.vue';
import ErrorView from '@/views/ErrorView.vue';
import FirmwareUpgradeView from '@/views/FirmwareUpgradeView.vue';
import HomeView from '@/views/HomeView.vue';
import VedirectAdminView from '@/views/VedirectAdminView.vue';
import PowerMeterAdminView from '@/views/PowerMeterAdminView.vue';
import PowerLimiterAdminView from '@/views/PowerLimiterAdminView.vue';
import VedirectInfoView from '@/views/VedirectInfoView.vue';
import InverterAdminView from '@/views/InverterAdminView.vue';
import LoginView from '@/views/LoginView.vue';
import MaintenanceRebootView from '@/views/MaintenanceRebootView.vue';
import MqttAdminView from '@/views/MqttAdminView.vue';
import MqttInfoView from '@/views/MqttInfoView.vue';
import NetworkAdminView from '@/views/NetworkAdminView.vue';
import NetworkInfoView from '@/views/NetworkInfoView.vue';
import NtpAdminView from '@/views/NtpAdminView.vue';
import NtpInfoView from '@/views/NtpInfoView.vue';
import SecurityAdminView from '@/views/SecurityAdminView.vue';
import SystemInfoView from '@/views/SystemInfoView.vue';

// Import the views from the Hoywei-Turbo-WebApp
import PowerMeterInverterAdminView from '@/Hoywei-Turbo-WebApp/views/PowerMeterInverterAdminView.vue';
import PowerMeterChargerAdminView from '@/Hoywei-Turbo-WebApp/views/PowerMeterChargerAdminView.vue';
import PowerMeterSolarAdminView from '@/Hoywei-Turbo-WebApp/views/PowerMeterSolarAdminView.vue';
import PriceProviderAdminView from '@/Hoywei-Turbo-WebApp/views/PriceProviderAdminView.vue';

import { createRouter, createWebHistory } from 'vue-router';


const router = createRouter({
    history: createWebHistory(import.meta.env.BASE_URL),
    linkActiveClass: 'active',
    routes: [
        {
            path: '/',
            name: 'Home',
            component: HomeView,
        },
        {
            path: '/login',
            name: 'Login',
            component: LoginView,
        },
        {
            path: '/error?status=:status&message=:message',
            name: 'Error',
            component: ErrorView,
        },
        {
            path: '/about',
            name: 'About',
            component: AboutView,
        },
        {
            path: '/info/network',
            name: 'Network',
            component: NetworkInfoView,
        },
        {
            path: '/info/system',
            name: 'System',
            component: SystemInfoView,
        },
        {
            path: '/info/ntp',
            name: 'NTP',
            component: NtpInfoView,
        },
        {
            path: '/info/mqtt',
            name: 'MqTT',
            component: MqttInfoView,
        },
        {
            path: '/info/console',
            name: 'Web Console',
            component: ConsoleInfoView,
        },
        {
            path: '/info/vedirect',
            name: 'VE.Direct',
            component: VedirectInfoView,
        },
        {
            path: '/settings/network',
            name: 'Network Settings',
            component: NetworkAdminView,
        },
        {
            path: '/settings/ntp',
            name: 'NTP Settings',
            component: NtpAdminView,
        },
        {
            path: '/settings/vedirect',
            name: 'VE.Direct Settings',
            component: VedirectAdminView,
        },
        {
            path: '/settings/powermeter',
            name: 'Power meter Settings',
            component: PowerMeterAdminView,
        },
        {
            path: '/settings/powermeterinverter',
            name: 'Inverter Power Meter Settings',
            component: PowerMeterInverterAdminView,
        },
        {
            path: '/settings/powermetercharger',
            name: 'Charger Power Meter Settings',
            component: PowerMeterChargerAdminView,
        },
        {
            path: '/settings/powermetersolar',
            name: 'Solar Power Meter Settings',
            component: PowerMeterSolarAdminView,
        },
        {
            path: '/settings/powerlimiter',
            name: 'Power limiter Settings',
            component: PowerLimiterAdminView,
        },
        {
            path: '/settings/battery',
            name: 'Battery Settings',
            component: BatteryAdminView,
        },
        {
            path: '/settings/chargerac',
            name: 'Charger Settings',
            component: AcChargerAdminView,
        },
        {
            path: '/settings/mqtt',
            name: 'MqTT Settings',
            component: MqttAdminView,
        },
        {
            path: '/settings/inverter',
            name: 'Inverter Settings',
            component: InverterAdminView,
        },
        {
            path: '/settings/dtu',
            name: 'DTU Settings',
            component: DtuAdminView,
        },
        {
            path: '/settings/device',
            name: 'Device Manager',
            component: DeviceAdminView,
        },
        {
            path: '/firmware/upgrade',
            name: 'Firmware Upgrade',
            component: FirmwareUpgradeView,
        },
        {
            path: '/settings/config',
            name: 'Config Management',
            component: ConfigAdminView,
        },
        {
            path: '/settings/security',
            name: 'Security',
            component: SecurityAdminView,
        },
        {
            path: '/maintenance/reboot',
            name: 'Device Reboot',
            component: MaintenanceRebootView,
        },
        {
            path: '/priceprovider',
            name: 'PriceProviderAdmin',
            component: PriceProviderAdminView,
        },
    ],
});

export default router;
