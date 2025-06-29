<template>
  <div class="container-fluid py-5">
    <div class="row justify-content-center">
      <div class="col-lg-8">
        <div class="text-center mb-5">
          <h1 class="display-4 text-primary mb-3">
            <i class="bi bi-info-circle"></i> About Vue Recipes
          </h1>
          <p class="lead text-muted">Your Advanced Recipe Website</p>
        </div>

        <div class="row">
          <!-- About Section -->
          <div class="col-md-6 mb-4">
            <div class="card h-100 shadow-sm">
              <div class="card-body">
                <h3 class="card-title text-primary">
                  <i class="bi bi-cup-hot me-2"></i>What is Vue Recipes?
                </h3>
                <p class="card-text">
                  Vue Recipes is an advanced recipe website that allows you to discover, search, and save recipes from around the world. 
                  The site offers a modern user experience with a clean design and advanced functionality.
                </p>
              </div>
            </div>
          </div>

          <!-- Features Section -->
          <div class="col-md-6 mb-4">
            <div class="card h-100 shadow-sm">
              <div class="card-body">
                <h3 class="card-title text-success">
                  <i class="bi bi-star me-2"></i>Main Features
                </h3>
                <ul class="list-unstyled">
                  <li class="mb-2">
                    <i class="bi bi-check-circle text-success me-2"></i>
                    Advanced recipe search
                  </li>
                  <li class="mb-2">
                    <i class="bi bi-check-circle text-success me-2"></i>
                    Filter by cuisine type and diet
                  </li>
                  <li class="mb-2">
                    <i class="bi bi-check-circle text-success me-2"></i>
                    Registration and login system
                  </li>
                  <li class="mb-2">
                    <i class="bi bi-check-circle text-success me-2"></i>
                    Personalized home page
                  </li>
                </ul>
              </div>
            </div>
          </div>

          <!-- Technology Section -->
          <div class="col-md-6 mb-4">
            <div class="card h-100 shadow-sm">
              <div class="card-body">
                <h3 class="card-title text-info">
                  <i class="bi bi-gear me-2"></i>Technologies
                </h3>
                <div class="row">
                  <div class="col-6">
                    <h6 class="text-muted">Frontend</h6>
                    <ul class="list-unstyled small">
                      <li>Vue.js 3</li>
                      <li>Bootstrap 5</li>
                      <li>Vue Router</li>
                      <li>Axios</li>
                    </ul>
                  </div>
                  <div class="col-6">
                    <h6 class="text-muted">Backend</h6>
                    <ul class="list-unstyled small">
                      <li>Node.js</li>
                      <li>Express</li>
                      <li>MongoDB</li>
                      <li>JWT</li>
                    </ul>
                  </div>
                </div>
              </div>
            </div>
          </div>

          <!-- Contact Section -->
          <div class="col-md-6 mb-4">
            <div class="card h-100 shadow-sm">
              <div class="card-body">
                <h3 class="card-title text-warning">
                  <i class="bi bi-envelope me-2"></i>Contact Us
                </h3>
                <p class="card-text">
                  Have questions or suggestions? We'd love to hear from you!
                </p>
                <div class="d-grid">
                  <a href="mailto:contact@vuerecipes.com" class="btn btn-outline-warning">
                    <i class="bi bi-envelope me-2"></i>Send us an email
                  </a>
                </div>
              </div>
            </div>
          </div>
        </div>

        <!-- Team Section -->
        <div class="card shadow-sm mt-4">
          <div class="card-body text-center">
            <h3 class="card-title text-primary mb-4">
              <i class="bi bi-people me-2"></i>Development Team
            </h3>
            <div class="row">
              <div class="col-md-4 mb-3">
                <div class="d-flex flex-column align-items-center">
                  <i class="bi bi-person-circle display-4 text-primary mb-2"></i>
                  <h5>Frontend Developer</h5>
                  <p class="text-muted small">Vue.js, Bootstrap, UX/UI</p>
                </div>
              </div>
              <div class="col-md-4 mb-3">
                <div class="d-flex flex-column align-items-center">
                  <i class="bi bi-server display-4 text-success mb-2"></i>
                  <h5>Backend Developer</h5>
                  <p class="text-muted small">Node.js, Express, MongoDB</p>
                </div>
              </div>
              <div class="col-md-4 mb-3">
                <div class="d-flex flex-column align-items-center">
                  <i class="bi bi-palette display-4 text-info mb-2"></i>
                  <h5>UX/UI Designer</h5>
                  <p class="text-muted small">Bootstrap, Responsive Design</p>
                </div>
              </div>
            </div>
          </div>
        </div>

        <!-- Back to Home -->
        <div class="text-center mt-5">
          <router-link to="/" class="btn btn-primary btn-lg">
            <i class="bi bi-house me-2"></i>Back to Home
          </router-link>
        </div>
        <!-- Developer credit -->
        <div class="text-center mt-3 text-muted small">
          Developed by Adva Shani and Lee
        </div>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  name: 'AboutPage',
  mounted() {
    // Update page title
    document.title = 'About - Vue Recipes';
  }
}
</script>

<style scoped>
.card {
  border: none;
  border-radius: 15px;
  transition: transform 0.2s, box-shadow 0.2s;
}

.card:hover {
  transform: translateY(-2px);
  box-shadow: 0 8px 25px rgba(0,0,0,0.1) !important;
}

.list-unstyled li {
  padding: 0.25rem 0;
}

.btn {
  border-radius: 8px;
  font-weight: 500;
}

.btn:hover {
  transform: translateY(-1px);
}
</style>
