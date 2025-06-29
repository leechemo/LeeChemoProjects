<template>
  <div class="container-fluid py-5">
    <div class="row justify-content-center">
      <div class="col-md-6 col-lg-4">
        <div class="card shadow">
          <div class="card-header bg-primary text-white text-center">
            <h3 class="mb-0">
              <i class="bi bi-person-plus"></i> Sign Up
            </h3>
          </div>
          <div class="card-body p-4">
            <form @submit.prevent="register" novalidate>
              <!-- Username Field -->
              <div class="mb-3">
                <label for="username" class="form-label">
                  <i class="bi bi-person"></i> Username
                </label>
                <input 
                  id="username"
                  v-model="state.username" 
                  type="text" 
                  class="form-control"
                  :class="{ 'is-invalid': v$.username.$error, 'is-valid': !v$.username.$error && state.username }"
                  placeholder="Enter username"
                  @blur="v$.username.$touch()"
                />
                <div v-if="v$.username.$error" class="invalid-feedback">
                  <div v-if="v$.username.required.$invalid">Username is required</div>
                  <div v-if="v$.username.minLength.$invalid">Username must be at least 3 characters</div>
                  <div v-if="v$.username.alphaNum.$invalid">Username can contain only letters and numbers</div>
                </div>
              </div>

              <!-- First Name Field -->
              <div class="mb-3">
                <label for="firstname" class="form-label">
                  <i class="bi bi-person"></i> First Name
                </label>
                <input
                  id="firstname"
                  v-model="state.firstname"
                  type="text"
                  class="form-control"
                  :class="{ 'is-invalid': v$.firstname.$error, 'is-valid': !v$.firstname.$error && state.firstname }"
                  placeholder="Enter first name"
                  @blur="v$.firstname.$touch()"
                />
                <div v-if="v$.firstname.$error" class="invalid-feedback">
                  <div v-if="v$.firstname.required.$invalid">First name is required</div>
                </div>
              </div>

              <!-- Last Name Field -->
              <div class="mb-3">
                <label for="lastname" class="form-label">
                  <i class="bi bi-person"></i> Last Name
                </label>
                <input
                  id="lastname"
                  v-model="state.lastname"
                  type="text"
                  class="form-control"
                  :class="{ 'is-invalid': v$.lastname.$error, 'is-valid': !v$.lastname.$error && state.lastname }"
                  placeholder="Enter last name"
                  @blur="v$.lastname.$touch()"
                />
                <div v-if="v$.lastname.$error" class="invalid-feedback">
                  <div v-if="v$.lastname.required.$invalid">Last name is required</div>
                </div>
              </div>

              <!-- Country Field -->
              <div class="mb-3">
                <label for="country" class="form-label">
                  <i class="bi bi-globe"></i> Country
                </label>
                <input
                  id="country"
                  v-model="state.country"
                  type="text"
                  class="form-control"
                  :class="{ 'is-invalid': v$.country.$error, 'is-valid': !v$.country.$error && state.country }"
                  placeholder="Enter country"
                  @blur="v$.country.$touch()"
                />
                <div v-if="v$.country.$error" class="invalid-feedback">
                  <div v-if="v$.country.required.$invalid">Country is required</div>
                </div>
              </div>

              <!-- Email Field -->
              <div class="mb-3">
                <label for="email" class="form-label">
                  <i class="bi bi-envelope"></i> Email Address
                </label>
                <input 
                  id="email"
                  v-model="state.email" 
                  type="email" 
                  class="form-control"
                  :class="{ 'is-invalid': v$.email.$error, 'is-valid': !v$.email.$error && state.email }"
                  placeholder="Enter email"
                  @blur="v$.email.$touch()"
                />
                <div v-if="v$.email.$error" class="invalid-feedback">
                  <div v-if="v$.email.required.$invalid">Email is required</div>
                  <div v-if="v$.email.email.$invalid">Invalid email address</div>
                </div>
              </div>

              <!-- Password Field -->
              <div class="mb-3">
                <label for="password" class="form-label">
                  <i class="bi bi-lock"></i> Password
                </label>
                <div class="input-group">
                  <input 
                    id="password"
                    v-model="state.password" 
                    :type="showPassword ? 'text' : 'password'" 
                    class="form-control"
                    :class="{ 'is-invalid': v$.password.$error, 'is-valid': !v$.password.$error && state.password }"
                    placeholder="Enter password"
                    @blur="v$.password.$touch()"
                  />
                  <button 
                    type="button" 
                    class="btn btn-outline-secondary"
                    @click="showPassword = !showPassword"
                  >
                    <i :class="showPassword ? 'bi bi-eye-slash' : 'bi bi-eye'"></i>
                  </button>
                </div>
                <div v-if="v$.password.$error" class="invalid-feedback">
                  <div v-if="v$.password.required.$invalid">Password is required</div>
                  <div v-if="v$.password.minLength.$invalid">Password must be at least 8 characters</div>
                  <div v-if="v$.password.hasUpperCase.$invalid">Password must contain at least one uppercase letter</div>
                  <div v-if="v$.password.hasNumber.$invalid">Password must contain at least one number</div>
                </div>
                <div class="form-text">
                  <small>Password must be at least 8 characters long, contain an uppercase letter and a number</small>
                </div>
              </div>

              <!-- Confirm Password Field -->
              <div class="mb-3">
                <label for="confirmPassword" class="form-label">
                  <i class="bi bi-lock-fill"></i> Confirm Password
                </label>
                <div class="input-group">
                  <input 
                    id="confirmPassword"
                    v-model="state.confirmPassword" 
                    :type="showConfirmPassword ? 'text' : 'password'" 
                    class="form-control"
                    :class="{ 'is-invalid': v$.confirmPassword.$error, 'is-valid': !v$.confirmPassword.$error && state.confirmPassword }"
                    placeholder="Re-enter password"
                    @blur="v$.confirmPassword.$touch()"
                  />
                  <button 
                    type="button" 
                    class="btn btn-outline-secondary"
                    @click="showConfirmPassword = !showConfirmPassword"
                  >
                    <i :class="showConfirmPassword ? 'bi bi-eye-slash' : 'bi bi-eye'"></i>
                  </button>
                </div>
                <div v-if="v$.confirmPassword.$error" class="invalid-feedback">
                  {{ v$.confirmPassword.$errors[0]?.$message }}
                </div>
              </div>

              <!-- Submit Button -->
              <div class="d-grid gap-2">
                <button 
                  type="submit" 
                  class="btn btn-primary btn-lg"
                  :disabled="loading"
                >
                  <span v-if="loading" class="spinner-border spinner-border-sm me-2" role="status"></span>
                  <i v-else class="bi bi-person-plus me-2"></i>
                  {{ loading ? 'Registering...' : 'Sign Up' }}
                </button>
              </div>
            </form>

            <!-- Login Link -->
            <div class="text-center mt-3">
              <p class="mb-0">
                Already have an account? 
                <router-link to="/login" class="text-decoration-none">
                  <i class="bi bi-box-arrow-in-right"></i> Log in here
                </router-link>
              </p>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import { reactive, ref } from 'vue';
import { useVuelidate } from '@vuelidate/core';
import { required, minLength, email, helpers } from '@vuelidate/validators';
import { useRouter } from 'vue-router';
import axios from 'axios';

export default {
  name: "RegisterPage",
  setup() {
    const router = useRouter();
    const loading = ref(false);
    const showPassword = ref(false);
    const showConfirmPassword = ref(false);

    const state = reactive({
      username: '',
      firstname: '',
      lastname: '',
      country: '',
      email: '',
      password: '',
      confirmPassword: '',
      profilePic: ''
    });

    // Custom validations
    const alphaNum = helpers.regex(/^[a-zA-Z0-9]+$/);
    const hasUpperCase = helpers.regex(/[A-Z]/);
    const hasNumber = helpers.regex(/[0-9]/);

    const passwordsMatch = (value) => {
      return value === state.password;
    };

    const rules = {
      username: { 
        required, 
        minLength: minLength(3),
        alphaNum
      },
      firstname: { required },
      lastname: { required },
      country: { required },
      email: { 
        required, 
        email 
      },
      password: { 
        required, 
        minLength: minLength(8),
        hasUpperCase,
        hasNumber
      },
      confirmPassword: {
        required,
        passwordsMatch: helpers.withMessage(
          'Passwords do not match',
          passwordsMatch
        )
      },
      profilePic: {}
    };

    const v$ = useVuelidate(rules, state);

    const register = async () => {
      const isValid = await v$.value.$validate();

      if (!isValid) {
        window.toast('Error', 'Please fix the form errors', 'danger');
        return;
      }

      loading.value = true;

      try {
        await axios.post('http://localhost:3001/Register', {
          username: state.username,
          firstname: state.firstname,
          lastname: state.lastname,
          country: state.country,
          email: state.email,
          password: state.password,
          profilePic: state.profilePic
        });

        window.toast("Registration successful", "Your account has been created. You can now log in", "success");
        router.push('/login');
      } catch (err) {
        const errorMessage = err.response?.data?.message || 'An error occurred during registration';
        window.toast("Registration Error", errorMessage, "danger");
      } finally {
        loading.value = false;
      }
    };

    return { 
      state, 
      v$, 
      register, 
      loading, 
      showPassword, 
      showConfirmPassword
    };
  }
};
</script>

<style scoped>
.card {
  border: none;
  border-radius: 15px;
}

.card-header {
  border-radius: 15px 15px 0 0 !important;
  border-bottom: none;
}

.form-control:focus {
  border-color: #0d6efd;
  box-shadow: 0 0 0 0.2rem rgba(13, 110, 253, 0.25);
}

.btn-primary {
  border-radius: 8px;
  font-weight: 600;
}

.btn-primary:hover {
  transform: translateY(-1px);
  box-shadow: 0 4px 12px rgba(13, 110, 253, 0.3);
}

.input-group .btn {
  border-left: none;
}

.input-group .form-control {
  border-right: none;
}

.input-group .form-control:focus {
  border-right: none;
  box-shadow: none;
}

.input-group .btn:focus {
  box-shadow: none;
}

.form-text {
  font-size: 0.8rem;
  color: #6c757d;
}

a {
  color: #0d6efd;
}

a:hover {
  color: #0a58ca;
}
</style>
