KVM_ISO_URL = https://muug.ca/mirror/fedora/linux/releases/30/Server/x86_64/iso/Fedora-Server-dvd-x86_64-30-1.2.iso
KVM_ISO_URL = https://ftp.nluug.nl/pub/os/Linux/distr/fedora/linux/releases/30/Server/x86_64/iso/Fedora-Server-dvd-x86_64-30-1.2.iso
KVM_KICKSTART_FILE = testing/libvirt/fedora30.ks
# LIE!
KVM_OS_VARIANT ?= fedora30
KVM_PACKAGE_INSTALL = dnf install -y
KVM_PACKAGE_UPGRADE = dnf upgrade -y
KVM_DEBUGINFO_INSTALL = dnf debuginfo-install -y
KVM_INSTALL_RPM_LIST = 'rpm -aq > /var/tmp/rpm-qa-fedora-updates.log'

# Force the NSS version - version 3.40 caused pluto to dump core while
# loading the NSS DB.  Versions 3.36 and 3.41 (current at time of
# writing) seem to work.

# NSS_VERSION = -3.36.0-1.0.fc28.x86_64
NSS_VERSION =

KVM_PACKAGES = \
    ElectricFence \
    audit-libs-devel \
    bind-utils	\
    bison \
    conntrack-tools \
    curl-devel \
    elfutils-libelf-devel \
    fipscheck-devel \
    flex \
    gcc \
    gdb \
    git \
    glibc-devel \
    hping3 \
    ike-scan \
    kernel-core \
    kernel-devel \
    kernel-headers \
    kernel-modules \
    kernel-modules-extra \
    iproute \
    ipsec-tools \
    iptables \
    ldns \
    ldns-devel \
    libcap-ng-devel \
    libfaketime \
    libevent-devel \
    libseccomp-devel \
    libselinux-devel \
    linux-firmware \
    lsof \
    mtr \
    nc \
    net-tools \
    network-scripts \
    nsd \
    nspr \
    nspr-devel \
    nss$(NSS_VERSION) \
    nss-devel$(NSS_VERSION) \
    nss-tools$(NSS_VERSION) \
    nss-softokn$(NSS_VERSION) \
    nss-softokn-freebl$(NSS_VERSION) \
    ocspd \
    openldap-devel \
    pam-devel \
    patch \
    perf \
    pexpect \
    policycoreutils-python-utils \
    psmisc \
    python2-pyOpenSSL \
    python3-pexpect \
    python-setproctitle \
    racoon2 \
    rpm-build \
    screen \
    strace \
    systemd-devel \
    tar \
    tcpdump \
    telnet \
    unbound \
    unbound-devel \
    unbound-libs \
    valgrind \
    vim-enhanced \
    wget \
    xl2tpd \
    xmlto \
    strongswan \
    libfaketime \


KVM_DEBUGINFO = \
    ElectricFence \
    audit-libs \
    conntrack-tools \
    cyrus-sasl \
    glibc \
    keyutils \
    krb5-libs \
    ldns \
    libcap-ng \
    libcom_err \
    libcurl \
    libevent \
    libevent-devel \
    libgcc \
    libidn \
    libseccomp \
    libselinux \
    libssh2 \
    nspr \
    nss$(NSS_VERSION) \
    nss-softokn$(NSS_VERSION) \
    nss-softokn-freebl$(NSS_VERSION) \
    nss-util$(NSS_VERSION) \
    ocspd \
    openldap \
    openssl-libs \
    pam \
    pcre \
    python-libs \
    sqlite \
    unbound-libs \
    xz-libs \
    zlib \
